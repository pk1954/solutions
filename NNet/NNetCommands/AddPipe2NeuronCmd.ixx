// AddPipe2NeuronCmd.ixx
//
// NNetCommands

module;

#include <cassert>
#include <memory>
#include <iostream>

export module NNetCommands:AddPipe2NeuronCmd;

import :NNetCommand;
import NNetModel;

using std::make_unique;
using std::unique_ptr;

export class AddPipe2NeuronCmd : public NNetCommand
{
public:
	AddPipe2NeuronCmd            // case 9:  Add new input for Neuron, creating new InputLine  
	(                                 
		NobId         const  id,
		MicroMeterPnt const& pos
	)
	  : m_neuronOld(*m_pNMWI->GetNobPtr<Neuron*>(id))
	{
		assert(m_neuronOld.IsNeuron());
		m_upPipe      = make_unique<Pipe>();
		m_upNeuronNew = make_unique<Neuron>(m_neuronOld.GetPos());
		m_upInputLine = make_unique<InputLine>(pos);

		m_upPipe->SetStartPnt(m_upInputLine.get());
		m_upPipe->SetEndPnt  (m_upNeuronNew.get());
		m_upPipe->PosChanged();

		m_upNeuronNew->SetAxon(m_neuronOld.GetAxon()); // add axon
		m_upNeuronNew->AddIncoming(&m_neuronOld);      // add existing inputs 
		m_upNeuronNew->AddIncoming(m_upPipe.get());    // add new Pipe

		m_upInputLine->SetPipe(m_upPipe.get());
	}

	~AddPipe2NeuronCmd() final = default;

	void Do() final
	{
		m_neuronOld.Apply2AllInPipes
		(
			[this](Pipe& pipe) 
			{ 
				pipe.SetEndPnt(m_upNeuronNew.get());
				pipe.PosChanged();
			}
		);
		m_pNMWI->Push2Model(move(m_upNeuronNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_upNeuronOld = m_pNMWI->RemoveFromModel<Neuron>(m_neuronOld);
	}

	void Undo() final
	{
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		m_upPipe      = m_pNMWI->PopFromModel<Pipe>();
		m_upNeuronNew = m_pNMWI->PopFromModel<Neuron>();
		m_neuronOld.Apply2AllInPipes
		(
			[this](Pipe& pipe) 
			{ 
				pipe.SetEndPnt(m_upNeuronOld.get()); 
				pipe.PosChanged();
			}
		);
		m_pNMWI->Restore2Model(move(m_upNeuronOld));
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << nobId << pos << endl;
		PushCommand(make_unique<AddPipe2NeuronCmd>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"AddPipe2Neuron" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			AddPipe2NeuronCmd::Push(id, umPnt);
		}
	} m_wrapper { NAME };

	Neuron              & m_neuronOld;
	unique_ptr<Neuron>    m_upNeuronOld;
	unique_ptr<Neuron>    m_upNeuronNew;
	unique_ptr<InputLine> m_upInputLine;
	unique_ptr<Pipe>      m_upPipe;
};

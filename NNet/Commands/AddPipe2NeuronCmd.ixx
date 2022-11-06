// AddPipe2NeuronCmd.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module AddPipe2NeuronCmd;

import Types;
import NNetCommand;
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
		ConnectOutgoing(*m_upPipe.get(), *m_upInputLine.get());
		ConnectIncoming(*m_upPipe.get(), *m_upNeuronNew.get());
		m_upNeuronNew->SetOutgoing(m_neuronOld);
	}

	~AddPipe2NeuronCmd() final = default;

	void Do() final
	{
		m_neuronOld.Apply2AllOutPipes([this](Pipe& p) { ConnectOutgoing(p, *m_upNeuronNew.get()); });
		m_pNMWI->Push2Model(move(m_upNeuronNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upInputLine));
		m_upNeuronOld = m_pNMWI->RemoveFromModel<Neuron>(m_neuronOld);
	}

	void Undo() final
	{
		m_upInputLine = m_pNMWI->PopFromModel<InputLine>();
		m_upPipe = m_pNMWI->PopFromModel<Pipe>();
		m_upNeuronNew = m_pNMWI->PopFromModel<Neuron>();
		m_neuronOld.Apply2AllOutPipes([this](Pipe& p) { ConnectOutgoing(p, *m_upNeuronOld.get()); });
		m_pNMWI->Restore2Model(move(m_upNeuronOld));
	}

private:

	Neuron        const & m_neuronOld;
	unique_ptr<Neuron>    m_upNeuronOld;
	unique_ptr<Neuron>    m_upNeuronNew;
	unique_ptr<InputLine> m_upInputLine;
	unique_ptr<Pipe>      m_upPipe;
};

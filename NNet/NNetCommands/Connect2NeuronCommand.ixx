// Connect2NeuronCommand.ixx
//
// NNetCommands

export module NNetCommands:Connect2NeuronCommand;  

import std;
import Debug;
import :NNetCommand;
import :NNetSound;

export class Connect2NeuronCommand : public NNetCommand
{
public:
	Connect2NeuronCommand    // case 3: Add InputLine to Neuron 
	(
		NobId const idSrc,
		NobId const idDst
	)
	  : m_outputLineSrc(*m_pNMWI->GetNobPtr<OutputLine*>(idSrc)),
		m_neuronDst    (*m_pNMWI->GetNobPtr<Neuron*>    (idDst))
	{
		MicroMeterPnt pos { m_neuronDst.GetPos() };
		m_upNeuronNew = make_unique<Neuron>(m_neuronDst);       // create copy of Neuron
		m_upNeuronNew->AddIncoming(m_outputLineSrc.GetPipe()); // add incoming pipe
	}

	void Do() final
	{
		Assert(m_upNeuronNew.get());
		m_upNeuronNew->Reconnect();
		m_upNeuronDst     = m_pNMWI->ReplaceInModel <Neuron>(move(m_upNeuronNew));
		m_upOutputLineSrc = m_pNMWI->RemoveFromModel<OutputLine>(m_outputLineSrc);
		Assert(m_upOutputLineSrc.get());
		Assert(m_upNeuronDst.get());
	}

	void Undo() final
	{
		m_upNeuronDst    ->Reconnect();
		m_upOutputLineSrc->Reconnect();
		m_upNeuronNew = m_pNMWI->ReplaceInModel<Neuron>(move(m_upNeuronDst));
		m_pNMWI->Restore2Model(move(m_upOutputLineSrc));
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const nobId1, NobId const nobId2)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId1 << nobId2 << endl;
		PushCommand(make_unique<Connect2NeuronCommand>(nobId1, nobId2));
		PlaySound(SNAP_IN_SOUND);
	}

private:

	inline static const wstring NAME { L"Connect2Neuron" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId const id1 { ScrReadNobId(script) };
			NobId const id2 { ScrReadNobId(script) };
			Connect2NeuronCommand::Push(id1, id2);
		}
	} m_wrapper { NAME };

	OutputLine & m_outputLineSrc;  // reference to original OutputLine
	Neuron     & m_neuronDst;      // reference to original Neuron

	unique_ptr<Neuron>     m_upNeuronNew     { nullptr };
	unique_ptr<OutputLine> m_upOutputLineSrc { nullptr };
	unique_ptr<Neuron>     m_upNeuronDst     { nullptr };
};

// Connect2NeuronCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module Connect2NeuronCommand;  

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

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
		assert(m_upNeuronNew);
		m_upNeuronNew->Reconnect();
		m_upNeuronDst     = m_pNMWI->ReplaceInModel <Neuron>(move(m_upNeuronNew));
		m_upOutputLineSrc = m_pNMWI->RemoveFromModel<OutputLine>(m_outputLineSrc);
		assert(m_upOutputLineSrc);
		assert(m_upNeuronDst);
	}

	void Undo() final
	{
		m_upNeuronDst    ->Reconnect();
		m_upOutputLineSrc->Reconnect();
		m_upNeuronNew = m_pNMWI->ReplaceInModel<Neuron>(move(m_upNeuronDst));
		m_pNMWI->Restore2Model(move(m_upOutputLineSrc));
	}

private:

	OutputLine & m_outputLineSrc;  // reference to original OutputLine
	Neuron     & m_neuronDst;      // reference to original Neuron

	unique_ptr<Neuron>     m_upNeuronNew     { nullptr };
	unique_ptr<OutputLine> m_upOutputLineSrc { nullptr };
	unique_ptr<Neuron>     m_upNeuronDst     { nullptr };
};

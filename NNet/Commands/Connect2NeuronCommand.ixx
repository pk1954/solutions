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
		m_neuronNew = make_unique<Neuron>(pos);      // create copy of Neuron
		m_neuronNew->AddIncoming(m_outputLineSrc);   // add incoming pipe
	}

	void Do() final
	{
		assert(m_neuronNew);
		m_neuronNew->Reconnect();
		m_upNeuronDst     = m_pNMWI->ReplaceInModel <Neuron>(move(m_neuronNew));
		m_upOutputLineSrc = m_pNMWI->RemoveFromModel<OutputLine>(m_outputLineSrc);
		assert(m_upOutputLineSrc);
		assert(m_upNeuronDst);
	}

	void Undo() final
	{
		m_upNeuronDst    ->Reconnect();
		m_upOutputLineSrc->Reconnect();
		m_neuronNew = m_pNMWI->ReplaceInModel<Neuron>(move(m_upNeuronDst));
		m_pNMWI->Restore2Model(move(m_upOutputLineSrc));
	}

private:

	OutputLine const & m_outputLineSrc;  // reference to original OutputLine
	Neuron     const & m_neuronDst;      // reference to original Neuron

	unique_ptr<Neuron>     m_neuronNew       { nullptr };
	unique_ptr<OutputLine> m_upOutputLineSrc { nullptr };
	unique_ptr<Neuron>     m_upNeuronDst     { nullptr };
};

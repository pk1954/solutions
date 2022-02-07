// AddIncoming2InputNeuronCmd.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "Knot.h"

class AddIncoming2InputNeuronCmd : public NNetCommand
{
public:
	AddIncoming2InputNeuronCmd
	(
		NobId         const   id, 
		MicroMeterPnt const & pos 
	)
	{ 
		assert(m_pNMWI->IsInputNeuron(id));

		m_upNeuron  = make_unique<Neuron>(*m_pNMWI->GetNobPtr<InputNeuron *>(id));
		m_upKnotNew = make_unique<Knot>(pos);
		m_upPipe    = MakePipe(m_upKnotNew.get(), m_upNeuron.get());
		m_upKnotNew->AddOutgoing(m_upPipe.get());
		m_upNeuron ->AddIncoming(m_upPipe.get());
	}

	~AddIncoming2InputNeuronCmd() final = default;

	void Do() final
	{ 
		m_upInputNeuron = m_pNMWI->ReplaceInModel<InputNeuron>(move(m_upNeuron));
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
	}

	void Undo() final 
	{ 
		m_upNeuron  = m_pNMWI->ReplaceInModel<Neuron>(move(m_upInputNeuron));
		m_upPipe    = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew = m_pNMWI->PopFromModel<Knot>();
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron { };
	unique_ptr<Neuron>      m_upNeuron      { };
	unique_ptr<Knot>        m_upKnotNew     { };
	unique_ptr<Pipe>        m_upPipe        { };
};


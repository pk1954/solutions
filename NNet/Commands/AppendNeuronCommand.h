// AppendNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

template<typename T>
class AppendNeuronCommand : public NNetCommand
{
public:
	AppendNeuronCommand(NobId const idKnot)
	{
		Knot const * pKnot { m_pNMWI->GetNobPtr<Knot *>(idKnot) };
		m_upNeuron = make_unique<T>(pKnot->GetPos());
		m_upNeuron->SetConnections(*pKnot);
		m_upNeuron->SetId(idKnot);
	}

	~AppendNeuronCommand() {}

	virtual void Do() 
	{ 
		m_upNeuron->Reconnect();
		m_upKnot = m_pNMWI->ReplaceInModel<T,Knot>(move(m_upNeuron));
	}

	virtual void Undo() 
	{ 
		m_upKnot->Reconnect();
		m_upNeuron = m_pNMWI->ReplaceInModel<Knot,T>(move(m_upKnot)); 
	}

private:
	unique_ptr<T>    m_upNeuron { nullptr };
	unique_ptr<Knot> m_upKnot   { nullptr };
};

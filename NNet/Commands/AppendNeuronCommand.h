// AppendNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

template<typename T>
class AppendNeuronCommand : public Command
{
public:
	AppendNeuronCommand
	(
		NNetModelWriterInterface & nmwi,
		NobId                const idKnot 
	)
	{
		Knot const * pKnot { nmwi.GetNobPtr<Knot *>(idKnot) };
		m_upNeuron = make_unique<T>(pKnot->GetPos());
		m_upNeuron->SetConnections(*pKnot);
		m_upNeuron->SetId(idKnot);
	}

	~AppendNeuronCommand() {}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_upNeuron->Reconnect();
		m_upKnot = nmwi.ReplaceInModel<T,Knot>(move(m_upNeuron));
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_upKnot->Reconnect();
		m_upNeuron = nmwi.ReplaceInModel<Knot,T>(move(m_upKnot)); 
	}

private:
	unique_ptr<T>    m_upNeuron { nullptr };
	unique_ptr<Knot> m_upKnot   { nullptr };
};

// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "PipeList.h"
#include "BaseKnot.h"

using std::swap;

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand
	(
		NobId const idSrc,
		NobId const idDst
	)
	  :	m_pBaseKnotSrc(m_pNMWI->GetNobPtr<BaseKnot *>(idSrc)),
		m_pBaseKnotDst(m_pNMWI->GetNobPtr<BaseKnot *>(idDst))
	{ 
		if (m_pBaseKnotDst->IsKnot())             // if a Neuron is connected to a Knot, the Knot would survive
			swap(m_pBaseKnotDst, m_pBaseKnotSrc); // swap makes sure, that the Neuron survives
		m_dstIncoming = m_pBaseKnotDst->GetIncoming();
		m_dstOutgoing = m_pBaseKnotDst->GetOutgoing();
	}

	~Connect2BaseKnotCommand()	{ }

	virtual void Do()
	{
		m_upBaseKnotSrc = m_pNMWI->RemoveFromModel<BaseKnot>(*m_pBaseKnotSrc); 
		assert(m_upBaseKnotSrc);
		m_pBaseKnotDst->AddIncoming(*m_upBaseKnotSrc.get());
		m_pBaseKnotDst->AddOutgoing(*m_upBaseKnotSrc.get());
		m_pBaseKnotDst->Reconnect();
	}

	virtual void Undo()
	{
		m_pBaseKnotDst->SetIncoming(m_dstIncoming);  // restore dst connections
		m_pBaseKnotDst->SetOutgoing(m_dstOutgoing); 
		assert(m_upBaseKnotSrc);
		m_upBaseKnotSrc->Reconnect();
		m_upBaseKnotSrc = m_pNMWI->ReplaceInModel<BaseKnot,BaseKnot>(move(m_upBaseKnotSrc)); // reconnect src  
	}

private:

	BaseKnot * m_pBaseKnotSrc;
	BaseKnot * m_pBaseKnotDst;

	unique_ptr<BaseKnot> m_upBaseKnotSrc { nullptr };
	PipeList             m_dstIncoming;
	PipeList             m_dstOutgoing;
};

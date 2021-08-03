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
		BaseKnot * pBaseKnotSrc,
		BaseKnot * pBaseKnotDst
	)
	  :	m_pBaseKnotSrc(pBaseKnotSrc),
		m_pBaseKnotDst(pBaseKnotDst)
	{ 
		if (m_pBaseKnotDst->IsKnot())           // if a Neuron is connected to a Knot, the Knot would survive
			swap(m_pBaseKnotDst, m_pBaseKnotSrc); // swap makes sure, that the Neuron survives
		m_dstIncoming = m_pBaseKnotDst->GetIncoming();
		m_dstOutgoing = m_pBaseKnotDst->GetOutgoing();
	}

	~Connect2BaseKnotCommand()	{ }

	virtual void Do(NNetModelWriterInterface & nmwi)
	{
		m_upBaseKnotSrc = nmwi.RemoveFromModel<BaseKnot>(*m_pBaseKnotSrc); 
		assert(m_upBaseKnotSrc);
		m_pBaseKnotDst->AddIncoming(*m_upBaseKnotSrc.get()); // double connections?
		m_pBaseKnotDst->AddOutgoing(*m_upBaseKnotSrc.get()); // double connections?
		m_pBaseKnotDst->Reconnect();
	}

	virtual void Undo(NNetModelWriterInterface & nmwi)
	{
		m_pBaseKnotDst->SetIncoming(m_dstIncoming);  // restore dst connections
		m_pBaseKnotDst->SetOutgoing(m_dstOutgoing); 
		assert(m_upBaseKnotSrc);
		m_upBaseKnotSrc->Reconnect();
		m_upBaseKnotSrc = nmwi.ReplaceInModel<BaseKnot,BaseKnot>(move(m_upBaseKnotSrc)); // reconnect src  
	}

private:

	BaseKnot * m_pBaseKnotSrc;
	BaseKnot * m_pBaseKnotDst;

	unique_ptr<BaseKnot> m_upBaseKnotSrc { nullptr };
	PipeList             m_dstIncoming;
	PipeList             m_dstOutgoing;
};

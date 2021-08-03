// Connect2PipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Pipe.h"

class Connect2PipeCommand : public Command
{
public:
	Connect2PipeCommand
	(
		BaseKnot * pBaseKnot,
		Pipe     * pPipe
	)
	:	m_pBaseKnot(pBaseKnot),
		m_pPipe    (pPipe)
	{
		m_pStartKnot = m_pPipe->GetStartKnotPtr();
		m_upNewPipe  = make_unique<Pipe>(m_pStartKnot, m_pBaseKnot);
		m_upNewPipe->Select(m_pPipe->IsSelected());
	}

	~Connect2PipeCommand()	{ }

	virtual void Do(NNetModelWriterInterface & nmwi)
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipe, m_upNewPipe.get());
		m_pBaseKnot ->AddIncoming(m_upNewPipe.get());
		m_pBaseKnot ->AddOutgoing(m_pPipe);
		m_pPipe->SetStartKnot(m_pBaseKnot);
		nmwi.Push2Model(move(m_upNewPipe));
	}

	virtual void Undo(NNetModelWriterInterface & nmwi)
	{
		m_upNewPipe = nmwi.PopFromModel<Pipe>();
		m_pStartKnot->ReplaceOutgoing(m_upNewPipe.get(), m_pPipe);
		m_pBaseKnot ->RemoveIncoming(m_upNewPipe.get());
		m_pBaseKnot ->RemoveOutgoing(m_pPipe);
		m_pPipe->SetStartKnot(m_pStartKnot);
	}

private:
	unique_ptr<Pipe> m_upNewPipe { nullptr };
	BaseKnot * const m_pBaseKnot;
	Pipe     * const m_pPipe;
	BaseKnot *       m_pStartKnot;
};

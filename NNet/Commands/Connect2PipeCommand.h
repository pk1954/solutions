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
	:	m_pBaseKnot( pBaseKnot ),
		m_pPipe    ( pPipe )
	{
		m_pStartKnot = m_pPipe->GetStartKnotPtr( );
		m_upNewPipe  = make_unique<Pipe>( m_pStartKnot, m_pBaseKnot );
	}

	~Connect2PipeCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		nmwi.GetShapes().Push( move(m_upNewPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upNewPipe = nmwi.GetShapes().Pop<Pipe>( );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upNewPipe.get(), m_pPipe );
		m_pBaseKnot ->m_connections.RemoveIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.RemoveOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pStartKnot );
	}

private:
	unique_ptr<Pipe> m_upNewPipe { nullptr };
	BaseKnot * const m_pBaseKnot;
	Pipe     * const m_pPipe;
	BaseKnot *       m_pStartKnot;
};

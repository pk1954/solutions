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
		NNetModelWriterInterface & model, 
		ShapeId              const idSrc,
		ShapeId              const idDst 
	)
	  :	m_pBaseKnot ( model.GetShapePtr<BaseKnot *>( idSrc ) ),
		m_pPipe     ( model.GetShapePtr<Pipe     *>( idDst ) )
	{ 
		m_pStartKnot = m_pPipe->GetStartKnotPtr( );
		m_pNewPipe   = model.NewPipe( m_pStartKnot, m_pBaseKnot );
	}

	~Connect2PipeCommand( )
	{ 
		delete m_pNewPipe;
	}

	virtual void Do( NNetModelWriterInterface & model )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_pNewPipe );
		m_pBaseKnot ->m_connections.AddIncoming( m_pNewPipe );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		model.Store2Model( m_pNewPipe );
	}

	virtual void Undo( NNetModelWriterInterface & model )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pNewPipe, m_pPipe );
		m_pBaseKnot ->m_connections.RemoveIncoming( m_pNewPipe );
		m_pBaseKnot ->m_connections.RemoveOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pStartKnot );
		model.RemoveFromModel( m_pNewPipe );
	}

private:
	BaseKnot * const m_pBaseKnot;
	Pipe     * const m_pPipe;
	BaseKnot *       m_pStartKnot;
	Pipe     *       m_pNewPipe;
};

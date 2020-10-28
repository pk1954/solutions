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
		NNetModelWriterInterface & nmwi, 
		ShapeId              const idSrc,
		ShapeId              const idDst 
	)
	  :	m_pBaseKnot ( nmwi.GetShapePtr<BaseKnot *>( idSrc ) ),
		m_pPipe     ( nmwi.GetShapePtr<Pipe     *>( idDst ) )
	{ 
		m_pStartKnot = m_pPipe->GetStartKnotPtr( );
		m_upNewPipe  = nmwi.NewPipe( m_pStartKnot, m_pBaseKnot );
	}

	~Connect2PipeCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		m_idNewPipe = nmwi.Add2Model( move(m_upNewPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upNewPipe = nmwi.RemoveFromModel<Pipe>( m_idNewPipe );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upNewPipe.get(), m_pPipe );
		m_pBaseKnot ->m_connections.RemoveIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.RemoveOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pStartKnot );
	}

private:
	BaseKnot * const m_pBaseKnot;
	Pipe     * const m_pPipe;
	unique_ptr<Pipe> m_upNewPipe  { nullptr };
	BaseKnot *       m_pStartKnot { nullptr };
	ShapeId          m_idNewPipe  { NO_SHAPE };
};

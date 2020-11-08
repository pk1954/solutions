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
		ShapeId const idSrc,
		ShapeId const idDst 
	)
	  : m_idSrc(idSrc),
		m_idDst(idDst)
	{ }

	~Connect2PipeCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		if ( ! m_upNewPipe )
		{
			m_pBaseKnot  = nmwi.GetShapePtr<BaseKnot *>( m_idSrc );
			m_pPipe      = nmwi.GetShapePtr<Pipe     *>( m_idDst );
			m_pStartKnot = m_pPipe->GetStartKnotPtr( );
			m_upNewPipe  = make_unique<Pipe>( m_pStartKnot, m_pBaseKnot );
		}

		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		nmwi.Push2Model( move(m_upNewPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_upNewPipe = nmwi.PopFromModel<Pipe>( );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upNewPipe.get(), m_pPipe );
		m_pBaseKnot ->m_connections.RemoveIncoming( m_upNewPipe.get() );
		m_pBaseKnot ->m_connections.RemoveOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pStartKnot );
	}

private:
	ShapeId    const m_idSrc;
	ShapeId    const m_idDst;
	unique_ptr<Pipe> m_upNewPipe  { nullptr };
	BaseKnot       * m_pBaseKnot  { nullptr };
	Pipe           * m_pPipe      { nullptr };
	BaseKnot       * m_pStartKnot { nullptr };
};

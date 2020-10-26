// AddIncoming2PipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class AddIncoming2PipeCommand : public Command
{
public:
	AddIncoming2PipeCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId            const   idPipe, 
		MicroMeterPoint    const & pos 
	)
	   : m_pPipeOld( nmwi.GetShapePtr<Pipe *>( idPipe ) )
	{
		m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
		m_upKnotInsert   = nmwi.NewBaseKnot<Knot>( pos );                                
		m_upKnotOrtho    = nmwi.NewBaseKnot<Knot>( pos - nmwi.OrthoVector( idPipe ) );

		m_upPipeOrtho    = nmwi.NewPipe( m_upKnotOrtho.get(), m_upKnotInsert.get() );		
		m_upPipeExt      = nmwi.NewPipe( m_pStartKnotOld,     m_upKnotInsert.get() );   	

		m_upKnotInsert->m_connections.AddIncoming( m_upPipeExt.get() );
		m_upKnotOrtho ->m_connections.AddOutgoing( m_upPipeOrtho.get() );

		m_upKnotInsert->m_connections.AddIncoming( m_upPipeOrtho.get() );
		m_upKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
	}

	~AddIncoming2PipeCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeOld, m_upPipeExt.get() );
		m_pPipeOld->SetStartKnot( m_upKnotInsert.get() );
		nmwi.Store2Model( move(m_upKnotOrtho) );
		nmwi.Store2Model( move(m_upKnotInsert) );
		nmwi.Store2Model( move(m_upPipeOrtho) );
		nmwi.Store2Model( move(m_upPipeExt) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_upPipeExt.get(), m_pPipeOld );
		m_pPipeOld->SetStartKnot( m_pStartKnotOld );
		m_upKnotOrtho  = move( nmwi.RemoveFromModel<Knot>( m_upKnotOrtho ->GetId() ) );
		m_upKnotInsert = move( nmwi.RemoveFromModel<Knot>( m_upKnotInsert->GetId() ) );
		m_upPipeOrtho  = move( nmwi.RemoveFromModel<Pipe>( m_upPipeOrtho ->GetId() ) );
		m_upPipeExt    = move( nmwi.RemoveFromModel<Pipe>( m_upPipeExt   ->GetId() ) );
	}

private:
	Pipe * const     m_pPipeOld;
	unique_ptr<Pipe> m_upPipeExt     { nullptr };
	unique_ptr<Pipe> m_upPipeOrtho   { nullptr };
	BaseKnot *       m_pStartKnotOld { nullptr };
	unique_ptr<Knot> m_upKnotInsert  { nullptr };
	unique_ptr<Knot> m_upKnotOrtho   { nullptr };
};

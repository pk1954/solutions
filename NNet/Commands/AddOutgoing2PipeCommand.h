// AddOutgoing2PipeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class AddOutgoing2PipeCommand : public Command
{
public:
	AddOutgoing2PipeCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId            const   idPipe, 
		MicroMeterPoint    const & pos 
	)
	  : m_pPipeOld( nmwi.GetShapePtr<Pipe *>( idPipe ) )
	{
		m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
		m_upKnotInsert  = nmwi.NewBaseKnot<Knot>( pos );
		m_upKnotOrtho   = nmwi.NewBaseKnot<Knot>( pos + nmwi.OrthoVector( idPipe ) );
		m_upPipeOrtho   = nmwi.NewPipe( m_upKnotInsert.get(), m_upKnotOrtho.get() );
		m_upPipeExt     = nmwi.NewPipe( m_pStartKnotOld, m_upKnotInsert.get() );	

		m_upKnotInsert->m_connections.AddOutgoing( m_upPipeOrtho.get() );
		m_upKnotOrtho ->m_connections.AddIncoming( m_upPipeOrtho.get() );
		m_upKnotInsert->m_connections.AddIncoming( m_upPipeExt.get() );
		m_upKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
	}

	~AddOutgoing2PipeCommand( )	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeOld, m_upPipeExt.get() );
		m_pPipeOld->SetStartKnot( m_upKnotInsert.get() );
		m_idKnotOrtho  = nmwi.Add2Model( move(m_upKnotOrtho ) );
		m_idKnotInsert = nmwi.Add2Model( move(m_upKnotInsert) );
		m_idPipeOrtho  = nmwi.Add2Model( move(m_upPipeOrtho ) );
		m_idPipeExt    = nmwi.Add2Model( move(m_upPipeExt   ) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnotOrtho  = nmwi.RemoveFromModel<Knot>( m_idKnotOrtho  );
		m_upKnotInsert = nmwi.RemoveFromModel<Knot>( m_idKnotInsert );
		m_upPipeOrtho  = nmwi.RemoveFromModel<Pipe>( m_idPipeOrtho  );
		m_upPipeExt    = nmwi.RemoveFromModel<Pipe>( m_idPipeExt    );
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_upPipeExt.get(), m_pPipeOld );
		m_pPipeOld->SetStartKnot( m_pStartKnotOld );
	}

private:
	Pipe     * const m_pPipeOld      { nullptr };
	BaseKnot *       m_pStartKnotOld { nullptr };
	unique_ptr<Pipe> m_upPipeExt     { nullptr };
	unique_ptr<Pipe> m_upPipeOrtho   { nullptr };
	unique_ptr<Knot> m_upKnotInsert  { nullptr };
	unique_ptr<Knot> m_upKnotOrtho   { nullptr }; 
	ShapeId          m_idPipeExt     { NO_SHAPE };
	ShapeId          m_idPipeOrtho   { NO_SHAPE };
	ShapeId          m_idKnotInsert  { NO_SHAPE };
	ShapeId          m_idKnotOrtho   { NO_SHAPE };
};

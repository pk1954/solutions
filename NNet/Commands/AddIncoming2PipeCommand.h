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
		NNetModelWriterInterface & model, 
		ShapeId            const   idPipe, 
		MicroMeterPoint    const & pos 
	)
	   : m_pPipeOld( model.GetShapePtr<Pipe *>( idPipe ) )
	{
		m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
		m_pKnotInsert   = model.NewBaseKnot<Knot>( pos );                                
		m_pKnotOrtho    = model.NewBaseKnot<Knot>( pos - model.OrthoVector( idPipe ) );

		m_pPipeOrtho    = model.NewPipe( m_pKnotOrtho,    m_pKnotInsert );		
		m_pPipeExt      = model.NewPipe( m_pStartKnotOld, m_pKnotInsert );   	

		m_pKnotInsert->m_connections.AddIncoming( m_pPipeExt );
		m_pKnotOrtho ->m_connections.AddOutgoing( m_pPipeOrtho );

		m_pKnotInsert->m_connections.AddIncoming( m_pPipeOrtho );
		m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
	}

	~AddIncoming2PipeCommand( )
	{
		delete m_pKnotInsert;
		delete m_pKnotOrtho;
		delete m_pPipeOrtho;
		delete m_pPipeExt;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeOld, m_pPipeExt );
		m_pPipeOld->SetStartKnot( m_pKnotInsert );
		model.Store2Model( m_pKnotOrtho );
		model.Store2Model( m_pKnotInsert );
		model.Store2Model( m_pPipeOrtho );
		model.Store2Model( m_pPipeExt );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeExt, m_pPipeOld );
		m_pPipeOld->SetStartKnot( m_pStartKnotOld );
		model.RemoveFromModel( m_pKnotOrtho  );
		model.RemoveFromModel( m_pKnotInsert );
		model.RemoveFromModel( m_pPipeOrtho  );
		model.RemoveFromModel( m_pPipeExt    );
	}

private:
	Pipe     * const m_pPipeOld;
	Pipe     *       m_pPipeExt      { nullptr };
	Pipe     *       m_pPipeOrtho    { nullptr };
	BaseKnot *       m_pStartKnotOld { nullptr };
	Knot     *       m_pKnotInsert   { nullptr };
	Knot     *       m_pKnotOrtho    { nullptr };
};

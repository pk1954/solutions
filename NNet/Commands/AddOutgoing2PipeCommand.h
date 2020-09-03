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
		NNetModelWriterInterface * const   pModel, 
		ShapeId                    const   idPipe, 
		MicroMeterPoint            const & pos 
	)
	{
		m_pPipeOld      = pModel->GetShapePtr<Pipe *>( idPipe );
		m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
		m_pKnotInsert   = pModel->NewBaseKnot<Knot>( pos );
		m_pKnotOrtho    = pModel->NewBaseKnot<Knot>( pos + pModel->OrthoVector( idPipe ) );
		m_pPipeOrtho    = pModel->NewPipe( m_pKnotInsert, m_pKnotOrtho );		
		m_pPipeExt      = pModel->NewPipe( m_pStartKnotOld, m_pKnotInsert );	

		m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOrtho );
		m_pKnotOrtho ->m_connections.AddIncoming( m_pPipeOrtho );
		m_pKnotInsert->m_connections.AddIncoming( m_pPipeExt );
		m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
	}

	~AddOutgoing2PipeCommand( )
	{
		delete m_pKnotInsert;
		delete m_pKnotOrtho;
		delete m_pPipeOrtho;
		delete m_pPipeExt;
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeOld, m_pPipeExt );
		m_pPipeOld->SetStartKnot( m_pKnotInsert );
		pModel->Store2Model( m_pKnotOrtho );
		pModel->Store2Model( m_pKnotInsert );
		pModel->Store2Model( m_pPipeOrtho );
		pModel->Store2Model( m_pPipeExt );

	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pStartKnotOld->m_connections.ReplaceOutgoing( m_pPipeExt, m_pPipeOld );
		m_pPipeOld->SetStartKnot( m_pStartKnotOld );

		pModel->RemoveFromModel( m_pKnotOrtho  );
		pModel->RemoveFromModel( m_pKnotInsert );
		pModel->RemoveFromModel( m_pPipeOrtho  );
		pModel->RemoveFromModel( m_pPipeExt    );
	}

private:
	Pipe     * m_pPipeOld      { nullptr };
	Pipe     * m_pPipeExt      { nullptr };
	Pipe     * m_pPipeOrtho    { nullptr };
	BaseKnot * m_pStartKnotOld { nullptr };
	Knot     * m_pKnotInsert   { nullptr };
	Knot     * m_pKnotOrtho    { nullptr };
};

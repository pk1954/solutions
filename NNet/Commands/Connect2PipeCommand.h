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
		NNetModelWriterInterface * const pModel, 
		ShapeId                    const idSrc,
		ShapeId                    const idDst 
	)
	  :	m_pBaseKnot ( pModel->GetShapePtr<BaseKnot *>( idSrc ) ),
		m_pPipe     ( pModel->GetShapePtr<Pipe     *>( idDst ) ),
		m_pNewPipe  ( pModel->NewPipe( m_pStartKnot, m_pBaseKnot ) ),
		m_pStartKnot( m_pPipe->GetStartKnotPtr( ) )
	{ }

	~Connect2PipeCommand( )
	{ 
		delete m_pNewPipe;
	}

	virtual void Do( NNetModelWriterInterface * const pModel )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_pNewPipe );
		m_pBaseKnot ->m_connections.AddIncoming( m_pNewPipe );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		pModel->Store2Model( m_pNewPipe );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pNewPipe, m_pPipe );
		m_pBaseKnot ->m_connections.RemoveIncoming( m_pNewPipe );
		m_pBaseKnot ->m_connections.RemoveOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pStartKnot );
		pModel->RemoveFromModel( m_pNewPipe );
	}

private:
	BaseKnot * m_pBaseKnot;
	Pipe     * m_pPipe;
	Pipe     * m_pNewPipe;
	BaseKnot * m_pStartKnot;
};

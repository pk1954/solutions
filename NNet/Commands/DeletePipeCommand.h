// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand( Pipe * pPipe )
		: m_pPipe( pPipe )
	{
		m_pPipe->CheckShape();
		m_pStartKnot = m_pPipe->GetStartKnotPtr();
		m_pEndKnot   = m_pPipe->GetEndKnotPtr();
	}

	~DeletePipeCommand( ){ }

	virtual void Do( NNetModelWriterInterface * const pModel )
	{
		m_pPipe->CheckShape();
		m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
		if ( m_pStartKnot->IsOrphanedKnot( ) )
			pModel->RemoveFromModel( m_pStartKnot );

		m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
		if ( m_pEndKnot->IsOrphanedKnot( ) )
			pModel->RemoveFromModel( m_pEndKnot );

		pModel->RemoveFromModel( m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel )
	{
		m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
		m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
		pModel->Store2Model( m_pStartKnot );
		pModel->Store2Model( m_pEndKnot );
		pModel->Store2Model( m_pPipe );
		m_pPipe->CheckShape();
	}

private:
	Pipe     * m_pPipe;
	BaseKnot * m_pStartKnot;
	BaseKnot * m_pEndKnot;
};

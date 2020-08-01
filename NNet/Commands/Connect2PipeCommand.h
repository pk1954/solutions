// Connect2PipeCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Pipe.h"

class Connect2PipeCommand : public Command
{
public:
	Connect2PipeCommand( NNetModel * const pModel, BaseKnot * const pSrc, Pipe * const pDst )
		:	m_pBaseKnot ( pSrc ),
		m_pPipe     ( pDst ),
		m_pNewPipe  ( nullptr ),
		m_pStartKnot( m_pPipe->GetStartKnotPtr( ) )
	{ 
		m_pNewPipe = pModel->NewPipe( m_pStartKnot, m_pBaseKnot );
	}

	~Connect2PipeCommand( )
	{ 
		delete m_pNewPipe;
	}

	virtual void Do( NNetModel * const pModel )
	{
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe, m_pNewPipe );
		m_pBaseKnot ->m_connections.AddIncoming( m_pNewPipe );
		m_pBaseKnot ->m_connections.AddOutgoing( m_pPipe );
		m_pPipe->SetStartKnot( m_pBaseKnot );
		pModel->Store2Model( m_pNewPipe );
	}

	virtual void Undo( NNetModel * const pModel )
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

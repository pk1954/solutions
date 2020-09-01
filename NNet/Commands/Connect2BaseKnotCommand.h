// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand( BaseKnot * const pSrc, BaseKnot * const pDst )
		:	m_pBaseKnotSrc( pSrc ),
			m_pBaseKnotDst( pDst ),
			m_pDstConnections( m_pBaseKnotDst->m_connections.Clone() )
	{ }

	~Connect2BaseKnotCommand( )
	{
		delete m_pBaseKnotSrc;
		delete m_pDstConnections;
	}

	virtual void Do( NNetModelWriterInterface * const pModel )
	{
		m_pBaseKnotDst->AddConnections( m_pBaseKnotSrc );
		pModel->RemoveFromModel( m_pBaseKnotSrc ); // m_pBaseKnotSrc disconnected, but unchanged. Do not delete, will be reused in Undo
	}

	virtual void Undo( NNetModelWriterInterface * const pModel )
	{
		m_pBaseKnotDst->SetConnections( m_pDstConnections );  // restore dst connections
		m_pBaseKnotSrc->RestoreConnections( );
		pModel->Store2Model( m_pBaseKnotSrc );           // reconnect src  
	}

private:
	BaseKnot    * m_pBaseKnotSrc;
	BaseKnot    * m_pBaseKnotDst;
	Connections * m_pDstConnections;
};

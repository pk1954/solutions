// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand
	( 
		NNetModelWriterInterface * const pModel, 
		ShapeId                    const idSrc,
		ShapeId                    const idDst 
	)
	  :	m_pBaseKnotSrc( pModel->GetShapePtr<BaseKnot *>( idSrc ) ),
		m_pBaseKnotDst( pModel->GetShapePtr<BaseKnot *>( idDst ) ),
		m_pDstConnections( m_pBaseKnotDst->m_connections.Clone() )
	{ }

	~Connect2BaseKnotCommand( )
	{
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
	BaseKnot    * const m_pBaseKnotSrc;
	BaseKnot    * const m_pBaseKnotDst;
	Connections * const m_pDstConnections;
};

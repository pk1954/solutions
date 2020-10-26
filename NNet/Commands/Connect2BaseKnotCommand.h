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
		NNetModelWriterInterface & nmwi, 
		ShapeId              const idSrc,
		ShapeId              const idDst 
	)
	  :	m_idSrc( idSrc ),
		m_idDst( idDst ),
		m_pBaseKnotDst( nmwi.GetShapePtr<BaseKnot *>( idDst ) )
	{
		m_pDstConnections = m_pBaseKnotDst->m_connections.Clone();
	}

	~Connect2BaseKnotCommand( )
	{
		delete m_pDstConnections;  // make to unique_ptr ???
	}

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		m_pBaseKnotDst->AddConnections( m_upBaseKnotSrc.get() );
		m_upBaseKnotSrc = move( nmwi.RemoveFromModel<BaseKnot>(m_idSrc) ); 
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_pBaseKnotDst->SetConnections( m_pDstConnections );  // restore dst connections
		m_upBaseKnotSrc->RestoreConnections( );
		m_upBaseKnotSrc = move(nmwi.Store2Model<BaseKnot>( move( m_upBaseKnotSrc ) )); // reconnect src  
	}

private:
	ShapeId const m_idSrc;
	ShapeId const m_idDst; 

	unique_ptr<BaseKnot> m_upBaseKnotSrc;
	BaseKnot    *        m_pBaseKnotDst;
	Connections *        m_pDstConnections;
};

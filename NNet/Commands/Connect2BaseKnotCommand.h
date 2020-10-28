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
		m_upDstConnections = m_pBaseKnotDst->m_connections.Clone();
	}

	~Connect2BaseKnotCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		m_upBaseKnotSrc = nmwi.RemoveFromModel<BaseKnot>(m_idSrc); 
		m_pBaseKnotDst->AddConnections( m_upBaseKnotSrc.get() );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_pBaseKnotDst->SetConnections( m_upDstConnections.get() );  // restore dst connections
		m_upBaseKnotSrc->Reconnect( );
		m_upBaseKnotSrc = nmwi.Store2Model<BaseKnot>( move( m_upBaseKnotSrc ) ); // reconnect src  
	}

private:
	ShapeId const m_idSrc;
	ShapeId const m_idDst; 

	BaseKnot              * m_pBaseKnotDst;
	unique_ptr<BaseKnot>    m_upBaseKnotSrc;
	unique_ptr<Connections> m_upDstConnections;
};

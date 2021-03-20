// MoveConnectorCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MoveCommand.h"
#include "Connector.h"

class MoveConnectorCommand : public MoveCommand
{
public:
	MoveConnectorCommand
	( 
		ShapeId         const   id, 
		MicroMeterPoint const & delta 
	)
	  : MoveCommand( delta ),
		m_idConnector( id )
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pConnector )
			m_pConnector = nmwi.GetShapePtr<Connector *>( m_idConnector );
		m_pConnector->MoveShape(m_delta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pConnector->MoveShape(-m_delta);
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_idConnector;
	}

private:
	ShapeId const m_idConnector;
	Connector   * m_pConnector { nullptr };
};

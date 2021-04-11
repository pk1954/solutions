// RotateShapeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Connector.h"
#include "Command.h"
#include "ShapeId.h"
#include "Shape.h"

class RotateConnectorCommand : public Command
{
public:
	RotateConnectorCommand
	( 
		ShapeId         const   idShape, 
		MicroMeterPoint const & umPntOld, 
		MicroMeterPoint const & umPntNew
	)
	  : m_idShape(idShape),
		m_umPntOld(umPntOld),
		m_umPntNew(umPntNew)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pConnector )
			m_pConnector = static_cast<Connector *>(nmwi.GetShapePtr<Shape *>( m_idShape ));
		m_pConnector->Rotate(m_umPntOld, m_umPntNew);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pConnector->Rotate(m_umPntNew, m_umPntOld);
	}

	virtual ShapeId const GetMovedShape() const
	{
		return m_idShape;
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

private:
	ShapeId         const m_idShape;
	MicroMeterPoint const m_umPntOld;
	MicroMeterPoint const m_umPntNew;
	Connector           * m_pConnector { nullptr };
};

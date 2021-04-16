// MoveShapeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "ShapeId.h"
#include "Shape.h"

class MoveShapeCommand : public Command
{
public:
	MoveShapeCommand
	( 
		ShapeId         const   idShape, 
		MicroMeterPoint const & delta 
	)
	  : m_delta( delta ),
		m_idShape( idShape )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pShape )
			m_pShape = nmwi.GetShapePtr<Shape *>( m_idShape );
		m_pShape->MoveShape(m_delta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pShape->MoveShape(-m_delta);
	}

	virtual ShapeId const GetMovedShape() const
	{
		return m_idShape;
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

	virtual bool const Combine(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		MoveShapeCommand const & srcCmd { static_cast<MoveShapeCommand const &>(src) };
		if (m_idShape != srcCmd.m_idShape)
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPoint       m_delta;
	ShapeId         const m_idShape;
	Shape               * m_pShape { nullptr };
};

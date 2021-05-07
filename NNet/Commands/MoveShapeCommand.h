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
		Shape                 & shape, 
		MicroMeterPoint const & delta 
	)
	  : m_delta( delta ),
		m_shape( shape )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.MoveShape(m_delta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.MoveShape(-m_delta);
	}

	virtual ShapeId const GetMovedShape() const
	{
		return m_shape.GetId();
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
		if (m_shape.GetId() != srcCmd.m_shape.GetId())
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPoint m_delta;
	Shape         & m_shape;
};

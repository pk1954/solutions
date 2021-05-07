// RotateCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Shape.h"

class RotateCommand : public Command
{
public:
	RotateCommand
	( 
		Shape                 & shape, 
		MicroMeterPoint const & umPntOld, 
		MicroMeterPoint const & umPntNew
	)
	  : m_shape(shape)
	{
		m_umPntPivot = shape.GetPos();
		Radian const radOld { Vector2Radian(umPntOld - m_umPntPivot) };
		Radian const radNew { Vector2Radian(umPntNew - m_umPntPivot) };
		m_radDelta = radNew - radOld;
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.RotateShape(m_umPntPivot, m_radDelta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.RotateShape(m_umPntPivot, -m_radDelta);
	}

	virtual ShapeId const GetMovedShape() const
	{
		return m_shape.GetId();
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

private:
	Shape         & m_shape;
	Radian          m_radDelta;
	MicroMeterPoint m_umPntPivot;
};

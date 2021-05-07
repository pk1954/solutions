// SetShapeCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "Connector.h"
#include "Command.h"
#include "ShapeId.h"
#include "Shape.h"

class SetShapeCommand : public Command
{
public:
	SetShapeCommand
	( 
		Shape                & shape, 
		MicroMeterPosDir const posDir
	)
      : m_shape(shape),
		m_posDir(posDir)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.RotateShape(m_shape .GetPos(), m_posDir.GetDir());
		m_shape.MoveShape  (m_posDir.GetPos() - m_shape.GetPos());
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_shape.RotateShape(m_shape.GetPos(), -m_posDir.GetDir());
		m_shape.MoveShape  (m_shape.GetPos() - m_posDir.GetPos());
	}

private:
	MicroMeterPosDir const m_posDir;
	Shape                & m_shape;
};

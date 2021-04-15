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
		ShapeId          const idShape, 
		MicroMeterPosDir const posDir
	)
      : m_idShape(idShape),
		m_posDir(posDir)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_pShape )
			m_pShape = nmwi.GetShapePtr<Shape *>( m_idShape );
		m_pShape->RotateShape(m_pShape->GetPos(), m_posDir.GetDir());
		m_pShape->MoveShape  (m_posDir.GetPos() - m_pShape->GetPos());
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pShape->RotateShape(m_pShape->GetPos(), -m_posDir.GetDir());
		m_pShape->MoveShape  (m_pShape->GetPos() - m_posDir.GetPos());
	}

private:
	ShapeId          const m_idShape;
	MicroMeterPosDir const m_posDir;
	Shape                * m_pShape { nullptr };
};

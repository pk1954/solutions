// SelectShapeCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "SelectionCommand.h"
#include "BaseKnot.h"

class SelectShapeCommand : public SelectionCommand
{
public:
	SelectShapeCommand
	( 
		ShapeId const id, 
		tBoolOp const op 
	)
	  :	m_idShape(id),
		m_op(op)
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		SelectionCommand::Do( nmwi );
		Shape * pShape { nmwi.GetShapePtr<Shape *>(m_idShape) };
		bool const bOn { ApplyOp2(pShape->IsSelected(), m_op) };
		pShape->Select(bOn, true);  // true: apply recursive 
	}

private:
	ShapeId const m_idShape;
	tBoolOp const m_op;
};

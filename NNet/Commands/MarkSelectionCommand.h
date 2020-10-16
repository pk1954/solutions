// MarkSelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "Shape.h"
#include "Command.h"
#include "BaseKnot.h"

using std::vector;

class MarkSelectionCommand : public Command
{
public:
	MarkSelectionCommand( NNetModelWriterInterface * pModel, tBoolOp const op )
	  : m_op( op )
	{
		pModel->GetShapeList( m_markedShapes, [&]( Shape const & s ){ return s.IsMarked(); } );
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( m_op ); } );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->Apply2All<Shape>( [&]( Shape & shape ) { shape.Mark( tBoolOp::opFalse ); } );
		m_markedShapes.Apply2All( [&]( Shape & shape ) { shape.Mark( tBoolOp::opTrue  ); } );
	}

private:
	ShapeList     m_markedShapes;
	tBoolOp const m_op;
};


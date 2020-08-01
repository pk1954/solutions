// MarkSelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "BoolOp.h"
#include "NNetModel.h"
#include "Shape.h"
#include "Command.h"
#include "BaseKnot.h"

using std::vector;

class MarkSelectionCommand : public Command
{
public:
	MarkSelectionCommand( NNetModel * pModel, tBoolOp const op )
	  : m_op( op ),
		m_markedShapes( pModel->GetShapeList( [&]( Shape const & s ){ return s.IsMarked(); } ) )
	{}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( m_op ); } );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		pModel->Apply2All<Shape>( [&]( Shape & shape ) { shape.Mark( tBoolOp::opFalse ); } );
		for ( Shape * pShape : m_markedShapes )
			pShape->Mark( tBoolOp::opTrue );
	}

private:
	vector<Shape *> m_markedShapes;
	tBoolOp const   m_op;
};


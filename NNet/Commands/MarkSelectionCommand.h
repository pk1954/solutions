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
	MarkSelectionCommand( NNetModelWriterInterface & model, tBoolOp const op )
	  : m_op( op )
	{
		model.Apply2All<Shape>                    
		(                                                        
			[&]( Shape & s )                 
			{
				if ( s.IsSelected() )
					m_markedShapes.push_back( & s );
			} 
		); 
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( m_op ); } );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		model.Apply2All<Shape>( [&]( Shape & shape ) { shape.Mark( tBoolOp::opFalse ); } );
		for ( Shape * pShape : m_markedShapes ) { pShape->Mark( tBoolOp::opTrue  ); };
	}

private:
	vector<Shape *> m_markedShapes;
	tBoolOp const   m_op;
};


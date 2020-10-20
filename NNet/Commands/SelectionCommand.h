// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Shape.h"
#include "Command.h"

using std::vector;

class SelectionCommand : public Command
{
public:
	SelectionCommand( NNetModelWriterInterface & model )
	{ 
		model.Apply2All<Shape>
		( 
			[&]( Shape & s )
			{ 
				if ( s.IsSelected() )
					m_selectedShapes.push_back( & s );
			} 
		);
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{
		model.SelectAllShapes( tBoolOp::opFalse );
		for (Shape * const pShape : m_selectedShapes)    
		{ 
			if ( pShape )
				pShape->Select( tBoolOp::opTrue ); 
		}
	}

protected:
	vector<Shape *> m_selectedShapes;
};


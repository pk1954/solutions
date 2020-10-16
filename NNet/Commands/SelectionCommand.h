// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Shape.h"
#include "ShapeList.h"
#include "Command.h"

using std::vector;

class SelectionCommand : public Command
{
public:
	SelectionCommand( NNetModelWriterInterface & model )
	{ 
		model.GetShapeList( m_selectedShapes, [&]( Shape const & s ){ return s.IsSelected(); } );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{
		model.SelectAllShapes( tBoolOp::opFalse );
		m_selectedShapes.SelectAllShapes( tBoolOp::opTrue );
	}

protected:
	ShapeList m_selectedShapes;
};


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
	SelectionCommand( NNetModelWriterInterface * const pModel )
	{ 
		pModel->GetShapeList( m_selectedShapes, [&]( Shape const & s ){ return s.IsSelected(); } );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		pModel->SelectAllShapes( tBoolOp::opFalse );
		m_selectedShapes.SelectAllShapes( tBoolOp::opTrue );
	}

protected:
	ShapeList m_selectedShapes;
};


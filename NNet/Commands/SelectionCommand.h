// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModel.h"
#include "Shape.h"
#include "Command.h"

using std::vector;

class SelectionCommand : public Command
{
public:
	SelectionCommand( NNetModelWriterInterface * const pModel )
		: m_selectedShapes { pModel->GetShapeList( [&]( Shape const & s ){ return s.IsSelected(); } ) }
	{ }

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		pModel->SelectAllShapes( tBoolOp::opFalse );
		for ( Shape * pShape : m_selectedShapes )
			pShape->Select( tBoolOp::opTrue );
	}

protected:
	vector<Shape *> m_selectedShapes;
};


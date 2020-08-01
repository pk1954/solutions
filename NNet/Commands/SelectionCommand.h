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
	SelectionCommand( NNetModel * const pModel )
		: m_selectedShapes { pModel->GetShapeList( [&]( Shape const & s ){ return s.IsSelected(); } ) }
	{ }

	virtual void Undo( NNetModel * const pModel ) 
	{
		pModel->SelectAll( tBoolOp::opFalse );
		for ( Shape * pShape : m_selectedShapes )
			pShape->Select( tBoolOp::opTrue );
	}

protected:
	vector<Shape *> m_selectedShapes;
};


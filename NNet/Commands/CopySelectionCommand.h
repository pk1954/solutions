// CopySelectionCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "Shape.h"
#include "SelectionCommand.h"

class CopySelectionCommand : public SelectionCommand
{
public:

	CopySelectionCommand( NNetModel * const pModel )
		:	SelectionCommand( pModel),
		m_copies ( pModel->DuplicateShapes( m_selectedShapes ) )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectAll( tBoolOp::opFalse );        // deselect all
		for ( Shape * pShape : m_copies )             // add copies
			if ( pShape )
				pModel->Store2Model( pShape );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		for ( Shape * pShape : m_copies )             // disconnect copies
			if ( pShape )
				pModel->RemoveFromModel( pShape );
		SelectionCommand::Undo( pModel );             // restore original selection
	}

private:
	NormalizedShapeList m_copies;
};


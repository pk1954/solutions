// CopySelectionCommand.h
//
// Commands

#pragma once

#include "Shape.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class CopySelectionCommand : public SelectionCommand
{
public:

	CopySelectionCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ 
		m_copies = pModel->GetModel().DuplicateShapes( m_selectedShapes );
		for ( Shape * pShape : m_copies )
		{
			if ( pShape && pShape->GetShapeType().IsBaseKnotType( ) )
				pShape->MoveShape( PIPE_WIDTH ); 
		};
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( tBoolOp::opFalse );  // deselect all
		for ( Shape * pShape : m_copies )             // add copies
			if ( pShape )
				pModel->Add2Model( pShape );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		for ( Shape * pShape : m_copies )             // disconnect copies
			if ( pShape )
				pModel->RemoveFromModel( pShape );
		SelectionCommand::Undo( pModel );             // restore original selection
	}

private:
	NormalizedShapeList m_copies;
};


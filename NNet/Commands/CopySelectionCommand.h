// CopySelectionCommand.h
//
// Commands

#pragma once

#include "Shape.h"
#include "ShapeList.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class CopySelectionCommand : public SelectionCommand
{
public:

	CopySelectionCommand( NNetModelWriterInterface * const pModel )
		:	SelectionCommand( pModel)
	{ 
		m_copies = m_selectedShapes;
		m_copies.Apply2All
		(
			[&]( Shape & shape )
			{
				if ( shape.GetShapeType().IsBaseKnotType( ) )
					shape.MoveShape( PIPE_WIDTH ); 
			}
		);
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( tBoolOp::opFalse );  // deselect all
		m_copies.Apply2All                            // add copies
		(
			[&]( Shape & shape ) { pModel->Add2Model( shape ); }
		);
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_copies.Apply2All     // disconnect copies
		(
			[&]( Shape & shape ) { pModel->RemoveFromModel( & shape ); }
		);
		SelectionCommand::Undo( pModel );             // restore original selection
	}

private:
	ShapeList m_copies { };
};


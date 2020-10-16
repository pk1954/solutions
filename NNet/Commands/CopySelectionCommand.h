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

	CopySelectionCommand( NNetModelWriterInterface & model )
		:	SelectionCommand( model)
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

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SelectAllShapes( tBoolOp::opFalse );  // deselect all
		m_copies.Apply2All                            // add copies
		(
			[&]( Shape & shape ) { model.Add2Model( shape ); }
		);
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_copies.Apply2All     // disconnect copies
		(
			[&]( Shape & shape ) { model.RemoveFromModel( & shape ); }
		);
		SelectionCommand::Undo( model );             // restore original selection
	}

private:
	ShapeList m_copies { };
};


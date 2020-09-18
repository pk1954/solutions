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
			[&]( Shape * pShape )
			{
				if ( pShape && pShape->GetShapeType().IsBaseKnotType( ) )
					pShape->MoveShape( PIPE_WIDTH ); 
			}
		);
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( tBoolOp::opFalse );  // deselect all
		m_copies.Apply2All                            // add copies
		(
			[&]( Shape * pShape )
			{
				if ( pShape )
					pModel->Add2Model( pShape );
			}
		);
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_copies.Apply2All     // disconnect copies
		(
			[&]( Shape * pShape )
			{
				if ( pShape )
					pModel->RemoveFromModel( pShape );
			}
		);
		SelectionCommand::Undo( pModel );             // restore original selection
	}

private:
	ShapeList m_copies { };
};


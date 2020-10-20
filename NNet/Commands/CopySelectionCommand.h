// CopySelectionCommand.h
//
// Commands

#pragma once

#include "Shape.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

class CopySelectionCommand : public Command
{
public:

	CopySelectionCommand( NNetModelWriterInterface & model )
	{ 
		model.Apply2All<Shape>
		(
			[&]( Shape & s )
			{
				if ( s.IsSelected( ) )
					m_selectedShapes.push_back( & s ); 
			}
		);
		for ( Shape * pShape : m_selectedShapes )
		{
			if ( pShape->GetShapeType().IsBaseKnotType( ) )
				pShape->MoveShape( PIPE_WIDTH ); 
		}
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SelectAllShapes( tBoolOp::opFalse );            // deselect all
		for ( Shape * pShape : m_selectedShapes )             // add copies
		{
			model.Add2Model( * pShape );
		}
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		for ( Shape * pShape : m_selectedShapes )    	// disconnect copies
		{
			model.RemoveFromModel( pShape );
		};
		model.SelectAllShapes( tBoolOp::opFalse );
		for (Shape * const pShape : m_selectedShapes)    
		{ 
			if ( pShape )
				pShape->Select( tBoolOp::opTrue ); 
		}
	}

private:
	vector<Shape *> m_selectedShapes;
};


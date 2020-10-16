// SelectShapeCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "SelectionCommand.h"
#include "BaseKnot.h"

class SelectShapeCommand : public SelectionCommand
{
public:
	SelectShapeCommand
	( 
		NNetModelWriterInterface & model, 
		ShapeId              const id, 
		tBoolOp              const op 
	)
	  :	SelectionCommand( model ),
		m_pShape( model.GetShapePtr<Shape *>( id ) ),
		m_op( op )
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		m_pShape->Select( m_op ); 
	}

private:
	Shape * m_pShape;
	tBoolOp m_op;
};

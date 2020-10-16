// SelectSubtreeCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "SelectionCommand.h"
#include "BaseKnot.h"

class SelectSubtreeCommand : public SelectionCommand
{
public:
	SelectSubtreeCommand
	( 
		NNetModelWriterInterface & model, 
		ShapeId              const id, 
		tBoolOp              const op 
	)
	  :	SelectionCommand( model),
		m_pBaseKnot( model.GetShapePtr<BaseKnot *>( id ) ),
		m_op( op )
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		model.SelectSubtree( m_pBaseKnot, m_op );
	}

private:
	BaseKnot    * m_pBaseKnot;
	tBoolOp const m_op;
};


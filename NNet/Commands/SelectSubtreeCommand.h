// SelectSubtreeCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModel.h"
#include "ShapeId.h"
#include "SelectionCommand.h"
#include "BaseKnot.h"

class SelectSubtreeCommand : public SelectionCommand
{
public:
	SelectSubtreeCommand( NNetModel * const pModel, ShapeId const id, tBoolOp const op )
		:	SelectionCommand( pModel),
		m_pBaseKnot( pModel->GetShapePtr<BaseKnot *>( id ) ),
		m_op( op )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectSubtree( m_pBaseKnot, m_op );
	}

private:
	BaseKnot    * m_pBaseKnot;
	tBoolOp const m_op;
};


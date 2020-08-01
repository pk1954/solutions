// SelectAllCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModel.h"
#include "SelectionCommand.h"

class SelectAllCommand : public SelectionCommand
{
public:
	SelectAllCommand( NNetModel * const pModel, tBoolOp const op )
		:	SelectionCommand( pModel),
		m_op( op )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->SelectAll( m_op ); 
	}

private:
	tBoolOp m_op;
};

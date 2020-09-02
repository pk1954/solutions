// SelectAllCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectAllCommand : public SelectionCommand
{
public:
	SelectAllCommand( NNetModelWriterInterface * const pModel, tBoolOp const op )
	  :	SelectionCommand( pModel),
		m_op( op )
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->SelectAllShapes( m_op ); 
	}

private:
	tBoolOp m_op;
};

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
	SelectAllCommand
	( 
		NNetModelWriterInterface & model, 
		tBoolOp const op 
	)
	  :	SelectionCommand( model),
		m_op( op )
	{ }

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.SelectAllShapes( m_op ); 
	}

private:
	tBoolOp m_op;
};

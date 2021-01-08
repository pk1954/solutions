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
	SelectAllCommand( tBoolOp const op )
	  :	m_op( op )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetShapes().SelectAllShapes( m_op ); 
	}

private:
	tBoolOp m_op;
};

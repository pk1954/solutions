// AddModelCommand.h
//
// Commands

#pragma once

#include "ShapeList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

class AddModelCommand : public SelectionCommand
{
public:

	AddModelCommand(  )
	{ 
	}

	virtual void Do( NNetModelWriterInterface * const pNMWI ) 
	{ 
		ShapeList & shapeListModel { pNMWI->GetShapes() };
		SelectionCommand::Do( * pNMWI );
		shapeListModel.SelectAllShapes( tBoolOp::opFalse );
		shapeListModel.Append( m_list );
	}

	virtual void Undo( NNetModelWriterInterface * const pNMWI ) 
	{ 
		SelectionCommand::Undo( * pNMWI );
		ShapeList & shapeListModel { pNMWI->GetShapes() };

	}

private:
	ShapeList m_list;
};


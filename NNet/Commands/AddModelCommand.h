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

	AddModelCommand( ShapeList const & list )
	{ 
		m_shapeList = list;
		m_shapeList.SelectAllShapes( tBoolOp::opTrue );
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetShapes().SelectAllShapes( tBoolOp::opFalse );
		m_idList = nmwi.GetShapes().Append( m_shapeList );
	}

	virtual void Undo( NNetModelWriterInterface  & nmwi ) 
	{ 
		m_shapeList = nmwi.GetShapes().ExtractShapes( m_idList );
		SelectionCommand::Undo( nmwi );
	}

private:
	ShapeList       m_shapeList;
	vector<ShapeId> m_idList;
};

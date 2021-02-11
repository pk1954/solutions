// AddModelCommand.h
//
// Commands

#pragma once

#include "UPShapeList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

class AddModelCommand : public SelectionCommand
{
public:

	AddModelCommand( UPShapeList const & list )
	{ 
		m_UPShapeList = list;
		m_UPShapeList.SelectAllShapes( tBoolOp::opTrue );
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetUPShapes().SelectAllShapes();
		m_idList = nmwi.GetUPShapes().Append( m_UPShapeList );
	}

	virtual void Undo( NNetModelWriterInterface  & nmwi ) 
	{ 
		m_UPShapeList = nmwi.GetUPShapes().ExtractShapes( m_idList );
		SelectionCommand::Undo( nmwi );
	}

private:
	UPShapeList     m_UPShapeList;
	vector<ShapeId> m_idList;
};

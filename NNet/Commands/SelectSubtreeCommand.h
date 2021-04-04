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
		ShapeId const id, 
		bool    const bOn
	)
	  :	m_idShape(id),
		m_bOn(bOn)
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.SelectSubtree(nmwi.GetShapePtr<BaseKnot *>(m_idShape), m_bOn);
	}

private:
	ShapeId const m_idShape;
	bool    const m_bOn;
};


// SelectShapesInRectCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectShapesInRectCommand : public SelectionCommand
{
public:
	SelectShapesInRectCommand
	( 
		MicroMeterRect const & rect,
		bool           const   bClear
	)
	  :	m_rect( rect ),
		m_bClearSelection( bClear )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		ShapeList & list { nmwi.GetShapes() };
		SelectionCommand::Do( nmwi );
		if ( m_bClearSelection )
			list.SelectAllShapes( tBoolOp::opFalse );
		list.Apply2AllInRect<Shape>(m_rect, [&](Shape& s){ s.Select(tBoolOp::opTrue); });
	}

private:
	MicroMeterRect const m_rect;
	bool           const m_bClearSelection;
};

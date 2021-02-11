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
		UPShapeList & list { nmwi.GetUPShapes() };
		SelectionCommand::Do( nmwi );
		if ( m_bClearSelection )
			list.DeselectAllShapes();
		list.Apply2AllInRect<Shape>(m_rect, [&](Shape& s){ s.Select(); });
	}

private:
	MicroMeterRect const m_rect;
	bool           const m_bClearSelection;
};

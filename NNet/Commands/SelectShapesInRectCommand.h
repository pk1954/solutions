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
		bool           const   bAdd2Selection
	)
	  :	m_rect( rect ),
		m_bAdd2Selection( bAdd2Selection )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		UPShapeList & list { nmwi.GetUPShapes() };
		SelectionCommand::Do( nmwi );
		if ( ! m_bAdd2Selection )
			list.DeselectAllShapes();
		list.Apply2AllInRect<Shape>(m_rect, [&](Shape& s){ s.Select(true, true); });
	}

private:
	MicroMeterRect const m_rect;
	bool           const m_bAdd2Selection;
};

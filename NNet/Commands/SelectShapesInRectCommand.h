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
	SelectShapesInRectCommand( MicroMeterRect const & rect )
	  :	m_rect( rect )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetShapes().Apply2AllInRect<Shape>
		( 
			m_rect, 
			[&]( Shape & shape ) 
			{ 
				shape.Select( tBoolOp::opTrue ); 
			} 
		);
	}

private:
	MicroMeterRect const m_rect;
};

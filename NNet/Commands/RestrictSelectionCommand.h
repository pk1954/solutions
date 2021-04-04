// RestrictSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class RestrictSelectionCommand : public SelectionCommand
{
public:
	RestrictSelectionCommand( ShapeType::Value const val )
		: m_val(val)
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		SelectionCommand::Do( nmwi );
		nmwi.GetUPShapes().Apply2AllSelected<Shape>
		( 
			[&](Shape & s)
			{ 
				if ( s.GetShapeType() != m_val ) 
					s.Select(false, false); 
			}
		);
	}

private:
	ShapeType::Value const m_val;
};

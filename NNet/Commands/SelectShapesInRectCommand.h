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
	SelectShapesInRectCommand( NNetModelWriterInterface & model, MicroMeterRect const & rect )
	  :	SelectionCommand( model),
		m_rect( rect )
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		model.Apply2AllInRect<Shape>( m_rect, [&]( Shape & shape ) { shape.Select( tBoolOp::opTrue ); } );
	}

private:
	MicroMeterRect const m_rect;
};

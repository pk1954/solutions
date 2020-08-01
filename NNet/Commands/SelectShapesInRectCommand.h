// SelectShapesInRectCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "SelectionCommand.h"

class SelectShapesInRectCommand : public SelectionCommand
{
public:
	SelectShapesInRectCommand( NNetModel * const pModel, MicroMeterRect const & rect )
	  :	SelectionCommand( pModel),
		m_rect( rect )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->Apply2AllInRect<Shape>( m_rect, [&]( Shape & shape ) { shape.Select( tBoolOp::opTrue ); } );
	}

private:
	MicroMeterRect const m_rect;
};

// MoveSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MoveCommand.h"
#include "BaseKnot.h"

class MoveSelectionCommand : public MoveCommand
{
public:
	MoveSelectionCommand( MicroMeterPoint const & delta )
       : MoveCommand( -delta )
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		MicroMeterPoint const delta = - m_delta;
		pModel->Apply2AllSelected<BaseKnot>
		( 
			[&]( BaseKnot & knot ) 
			{ 
				knot.MoveShape( delta ); 
			} 
		);
		m_delta = delta;
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return NO_SHAPE;
	}
};

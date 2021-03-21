// MoveSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "BaseKnot.h"

class MoveSelectionCommand : public Command
{
public:
	MoveSelectionCommand( MicroMeterPoint const & delta )
       : m_delta( -delta )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		MicroMeterPoint const delta = - m_delta;
		nmwi.GetUPShapes().Apply2AllSelected<Shape>
		( 
			[&](Shape & shape) 
			{ 
				if ( shape.IsBaseKnot() || shape.IsConnector() )
					shape.MoveShape( delta ); 
			} 
		);
		m_delta = delta;
	}

	virtual bool IsMoveCommand( ) const
	{ 
		return true; 
	};

private:
	MicroMeterPoint m_delta;
};

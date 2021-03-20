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
		nmwi.GetUPShapes().Apply2AllSelected<BaseKnot>
		( 
			[&]( BaseKnot & knot ) 
			{ 
				knot.MoveShape( delta ); 
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

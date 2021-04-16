// MoveSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
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
				if (shape.IsBaseKnot())
					shape.MoveShape(delta);
			} 
		);
		m_delta = delta;
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

	virtual bool const Combine(Command const & src) 
	{ 
		if (typeid(src) != typeid(this))
			return false;
		m_delta += static_cast<MoveSelectionCommand const &>(src).m_delta;
		return true; 
	};

private:
	MicroMeterPoint m_delta;
};

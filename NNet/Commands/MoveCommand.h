// MoveCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "Command.h"

class MoveCommand : public Command
{
public:
	MoveCommand( MicroMeterPoint const & delta )
	  : m_delta( delta )
	{}

	virtual bool IsMoveCommand( ) const
	{ 
		return true; 
	};

	virtual ShapeId const GetMovedShape( ) const = 0;

protected:
	MicroMeterPoint m_delta;
};

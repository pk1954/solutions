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

	MicroMeterPoint const GetDelta( ) const
	{
		return m_delta;
	}

	void AddDelta( MoveCommand const * const other)
	{
		m_delta += other->m_delta;
	}

protected:
	MicroMeterPoint m_delta;
};

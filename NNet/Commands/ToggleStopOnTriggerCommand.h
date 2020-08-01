// ToggleStopOnTriggerCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"

class ToggleStopOnTriggerCommand : public Command
{
public:
	ToggleStopOnTriggerCommand( ShapeId const id )
		:	m_id( id )
	{ }

	virtual void Do( NNetModel * const pModel )
	{
		pModel->ToggleStopOnTrigger( m_id );
	}

private:
	ShapeId const m_id;
};


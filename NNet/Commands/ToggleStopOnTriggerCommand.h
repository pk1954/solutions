// ToggleStopOnTriggerCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"

class ToggleStopOnTriggerCommand : public Command
{
public:
	ToggleStopOnTriggerCommand( ShapeId const id )
		:	m_id( id )
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel )
	{
		pModel->ToggleStopOnTrigger( m_id );
	}

private:
	ShapeId const m_id;
};


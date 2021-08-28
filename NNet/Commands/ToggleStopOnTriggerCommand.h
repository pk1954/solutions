// ToggleStopOnTriggerCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"

class ToggleStopOnTriggerCommand : public Command
{
public:
	ToggleStopOnTriggerCommand(NobId const id)
		:	m_id(id)
	{ }

	virtual void Do()
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

	virtual void Undo()
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

private:
	NobId const m_id;
};


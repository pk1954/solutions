// ToggleStopOnTriggerCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "NNetCommand.h"

class ToggleStopOnTriggerCommand : public NNetCommand
{
public:
	explicit ToggleStopOnTriggerCommand(NobId const id)
		:	m_id(id)
	{ }

	void Do() final
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

	void Undo() final
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

private:
	NobId const m_id;
};


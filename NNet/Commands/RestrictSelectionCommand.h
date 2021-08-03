// RestrictSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class RestrictSelectionCommand : public SelectionCommand
{
public:
	RestrictSelectionCommand(NobType::Value const val)
		: m_val(val)
	{ }

	virtual void Do(NNetModelWriterInterface & nmwi)
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & s)
			{ 
				if (s.GetNobType() != m_val) 
					s.Select(false); 
			}
		);
	}

private:
	NobType::Value const m_val;
};

// RestrictSelectionCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"

import MoreTypes;
import SelectionCommand;

class RestrictSelectionCommand : public SelectionCommand
{
public:
	explicit RestrictSelectionCommand(NobType::Value const val)
		: m_val(val)
	{ }

	void Do() final
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>
		(
			[this](Nob & s)
			{ 
				if (s.GetNobType() != m_val) 
					s.Select(false); 
			}
		);
	}

private:
	NobType::Value const m_val;
};

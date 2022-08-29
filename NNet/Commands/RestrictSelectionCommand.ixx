// RestrictSelectionCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module RestrictSelectionCommand;

import MoreTypes;
import SelectionCommand;
import NobType;

export class RestrictSelectionCommand : public SelectionCommand
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
			[this](Nob& s)
			{
				if (s.GetNobType() != m_val)
					s.Select(false);
			}
		);
	}

private:
	NobType::Value const m_val;
};

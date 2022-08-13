// SelectAllCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module SelectAllCommand;

import SelectionCommand;

export class SelectAllCommand : public SelectionCommand
{
public:
	SelectAllCommand(bool const bOn)
		:	m_bOn(bOn)
	{ }

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().SelectAllNobs(m_bOn); 
	}

private:
	bool m_bOn;
};

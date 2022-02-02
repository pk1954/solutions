// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

class SelectionCommand : public NNetCommand
{
public:

	void Undo() override
	{
		m_pNMWI->DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

private:
	unique_ptr<vector<Nob *>> m_upSelectedNobs { m_pNMWI->GetSelection() };
};


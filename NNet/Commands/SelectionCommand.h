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
	SelectionCommand()
	  : m_upSelectedNobs(move(m_pNMWI->GetSelection()))
	{}

	void Do() override 
	{}

	void Undo() override
	{
		m_pNMWI->DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

protected:
	unique_ptr<vector<Nob *>> m_upSelectedNobs;
};


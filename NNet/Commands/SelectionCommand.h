// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"

class SelectionCommand : public Command
{
public:
	SelectionCommand()
	  : m_upSelectedNobs(move(m_pNMWI->GetSelection()))
	{}

	virtual void Do() 
	{}

	virtual void Undo() 
	{
		m_pNMWI->DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

protected:
	unique_ptr<vector<Nob *>> m_upSelectedNobs;
};


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

	virtual void Do() 
	{
		if (! m_bInitialized)	
		{ 
			m_upSelectedNobs = move(m_pNMWI->GetSelection());
			m_bInitialized   = true;
		}
	}

	virtual void Undo() 
	{
		m_pNMWI->DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

protected:
	unique_ptr<vector<Nob *>> m_upSelectedNobs;
	bool                      m_bInitialized { false };
};


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

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{
		if (! m_bInitialized)	
		{ 
			m_upSelectedNobs = move(nmwi.GetSelection());
			m_bInitialized   = true;
		}
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{
		nmwi.GetUPNobs().DeselectAllNobs();
		for (auto it : *m_upSelectedNobs){ it->Select(true); };
	}

protected:
	unique_ptr<vector<Nob *>> m_upSelectedNobs;
	bool                      m_bInitialized { false };
};


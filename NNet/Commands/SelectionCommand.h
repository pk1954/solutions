// SelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "IoNeuronList.h"
#include "Command.h"

class SelectionCommand : public Command
{
public:

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{
		if ( ! m_bInitialized )	
		{ 
			m_selectedNobs = nmwi.GetSelection();
			m_bInitialized = true;
		}
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{
		nmwi.GetUPNobs().DeselectAllNobs();
		for (auto it : m_selectedNobs){ it->Select(true); };
	}

protected:
	vector<Nob *> m_selectedNobs;
	bool          m_bInitialized { false };
};


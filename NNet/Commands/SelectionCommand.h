// SelectionCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobPtrList.h"
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
		m_selectedNobs.Apply2All([&](Nob &s){ s.Select(true); });
	}

protected:
	NobPtrList<Nob> m_selectedNobs;
	bool            m_bInitialized { false };
};


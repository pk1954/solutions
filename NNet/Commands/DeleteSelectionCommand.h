// DeleteSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "CommandFunctions.h"
#include "NobIdList.h"
#include "CommandStack.h"
#include "Command.h"

class DeleteSelectionCommand : public Command
{
public:

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		if ( ! m_bInitialized )
		{
			init(nmwi);
			m_bInitialized = true;
		}
		m_cmdStack.DoAll();
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.UndoAll();
	}

private:
	void init(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.Initialize(&nmwi, nullptr);

		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob) 
			{ 
				m_cmdStack.Push( move( MakeDeleteCommand(nob) ) );
			}
		); 
	}

	CommandStack m_cmdStack     {};
	NobIdList  m_idList       {};
	bool         m_bInitialized { false };
};
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
	DeleteSelectionCommand(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.Initialize(&nmwi, nullptr);
		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob) 
			{ 
				unique_ptr<Command> upCmd { move(MakeDeleteCommand(nmwi, nob.GetId())) };
				if (upCmd)
					m_cmdStack.Push(move(upCmd));
			}
		); 
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.DoAll();
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.UndoAll();
	}

private:

	CommandStack m_cmdStack     {};
	NobIdList    m_idList       {};
	bool         m_bInitialized { false };
};
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
	{}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		//nmwi.DUMP();
		//nmwi.CheckModel();
		m_cmdStack.Initialize(&nmwi, nullptr);
		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob) 
			{ 
				if (unique_ptr<Command> upCmd { move(MakeDeleteCommand(nmwi, nob)) })
				{
					upCmd->Do(nmwi);
					m_cmdStack.Push(move(upCmd));
				}
			}
		); 
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_cmdStack.UndoAll();
		m_cmdStack.Clear();
	}

private:

	CommandStack m_cmdStack     {};
	NobIdList    m_idList       {};
	bool         m_bInitialized { false };
};
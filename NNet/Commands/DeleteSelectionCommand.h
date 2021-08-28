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
	DeleteSelectionCommand() 
	{}

	virtual void Do() 
	{ 
		m_cmdStack.Initialize(m_pNMWI, nullptr);
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob) 
			{ 
				if (unique_ptr<Command> upCmd { move(MakeDeleteCommand(*m_pNMWI, nob)) })
				{
					upCmd->Do();
					m_cmdStack.Push(move(upCmd));
				}
			}
		); 
	}

	virtual void Undo() 
	{ 
		m_cmdStack.UndoAll();
		m_cmdStack.Clear();
	}

private:

	CommandStack m_cmdStack     {};
	NobIdList    m_idList       {};
	bool         m_bInitialized { false };
};
// DeleteSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "CommandFunctions.h"
#include "NobIdList.h"
#include "CommandStack.h"
#include "NNetCommand.h"

class DeleteSelectionCommand : public NNetCommand
{
public:
	DeleteSelectionCommand() 
	{}

	void Do() final 
	{ 
		m_cmdStack.Initialize(m_pNMWI, nullptr);
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob) 
			{ 
				if (unique_ptr<NNetCommand> upCmd { move(MakeDeleteCommand(*m_pNMWI, nob)) })
				{
					upCmd->Do();
					m_pNMWI->CheckModel();
					m_cmdStack.Push(move(upCmd));
				}
			}
		); 
	}

	void Undo() final 
	{ 
		m_cmdStack.UndoAll();
		m_cmdStack.Clear();
	}

private:

	CommandStack m_cmdStack {};
};
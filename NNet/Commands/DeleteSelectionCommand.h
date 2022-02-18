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
	DeleteSelectionCommand() = default;

	void Do() final 
	{ 
		m_cmdStack.Initialize(m_pNMWI, nullptr);
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>( [this](Nob & nob) { doDelete(nob); });
	}

	void Undo() final 
	{ 
		m_cmdStack.UndoAll(); 
		m_cmdStack.Clear();
	}

private:

	void doDelete(Nob & nob) 
	{ 
		if (unique_ptr<NNetCommand> upCmd { MakeDeleteCommand(*m_pNMWI, nob) })
		{
			upCmd->Do();
			m_pNMWI->CheckModel();
			m_cmdStack.Push(move(upCmd));
		}
	}

	CommandStack m_cmdStack {};
};
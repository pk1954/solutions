// AddNobsCommand.h
//
// Commands

#pragma once

#include "UPNobList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

class AddNobsCommand : public SelectionCommand
{
public:

	AddNobsCommand(UPNobList nobs2Add)
	{ 
		m_nobs2Add = move(nobs2Add);
		m_nrOfNobs = m_nobs2Add.Size();
		m_nobs2Add.SelectAllNobs(true);
		m_nobs2Add.CheckNobList();
	}

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->DeselectAllNobs();
		m_pNMWI->GetUPNobs().MoveFrom(m_nobs2Add, m_nrOfNobs);
		m_pNMWI->CheckModel();
	}

	virtual void Undo() 
	{ 
		m_nobs2Add.MoveFrom(m_pNMWI->GetUPNobs(), m_nrOfNobs);
		SelectionCommand::Undo();
	}

private:
	UPNobList m_nobs2Add;
	size_t    m_nrOfNobs;
};

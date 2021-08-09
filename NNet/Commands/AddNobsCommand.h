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

	AddNobsCommand
	(
		NNetModelWriterInterface & nmwi,
		UPNobList                  nobs2Add
	)
	{ 
		m_nobs2Add = move(nobs2Add);
		m_nrOfNobs = m_nobs2Add.Size();
		m_nobs2Add.SelectAllNobs(true);
		m_nobs2Add.CheckNobList();
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.GetUPNobs().DeselectAllNobs();
		nmwi.GetUPNobs().MoveFrom(m_nobs2Add, m_nrOfNobs);
		nmwi.CheckModel();
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_nobs2Add.MoveFrom(nmwi.GetUPNobs(), m_nrOfNobs);
		SelectionCommand::Undo(nmwi);
	}

private:
	UPNobList m_nobs2Add;
	size_t    m_nrOfNobs;
};

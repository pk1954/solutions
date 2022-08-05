// AddNobsCommand.ixx
//
// Commands

module;

#include "UPNobList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

export module AddNobsCommand;

export class AddNobsCommand : public SelectionCommand
{
public:

	explicit AddNobsCommand(UPNobList const & nobs2Add)
		: m_nobs2Add(nobs2Add),
		m_nrOfNobs(nobs2Add.Size())
	{ 
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

	void Undo() final
	{ 
		m_nobs2Add.MoveFrom(m_pNMWI->GetUPNobs(), m_nrOfNobs);
		SelectionCommand::Undo();
	}

private:
	UPNobList m_nobs2Add;
	size_t    m_nrOfNobs;
};

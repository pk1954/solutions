// AddModelCommand.h
//
// Commands

#pragma once

#include "UPNobList.h"
#include "NobIdList.h"
#include "SelectionCommand.h"
#include "NNetModelWriterInterface.h"

class AddModelCommand : public SelectionCommand
{
public:

	AddModelCommand
	(
		NNetModelWriterInterface & nmwi,
		unique_ptr<NNetModel>      upImportedModel
	)
	  : m_upImportedModel(move(upImportedModel))
	{ 
		m_nrOfNobs = m_upImportedModel->Size();
		m_upImportedModel->GetUPNobs().SelectAllNobs(true);
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.GetUPNobs().SelectAllNobs(false);
		UPNobList & importedList { m_upImportedModel->GetUPNobs() };
		while (importedList.IsNotEmpty())
			nmwi.Push2Model(move(importedList.Pop<Nob>()));
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		UPNobList & importedList { m_upImportedModel->GetUPNobs() };
		for (size_t i = 0; i < m_nrOfNobs; ++i)
			importedList.Push(move(nmwi.PopFromModel<Nob>()));
		SelectionCommand::Undo(nmwi);
	}

private:
	unique_ptr<NNetModel> m_upImportedModel;
	size_t                m_nrOfNobs;
};

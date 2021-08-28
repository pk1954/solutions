// SelectSubtreeCommand.h
//
// Commands

#pragma once

#include "BoolOp.h"
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "SelectionCommand.h"
#include "BaseKnot.h"

class SelectSubtreeCommand : public SelectionCommand
{
public:
	SelectSubtreeCommand
	(
		NobId const id, 
		bool  const bOn
	)
	  :	m_idNob(id),
		m_bOn(bOn)
	{ }

	virtual void Do()
	{ 
		SelectionCommand::Do();
		m_pNMWI->SelectSubtree(m_pNMWI->GetNobPtr<BaseKnot *>(m_idNob), m_bOn);
	}

private:
	NobId const m_idNob;
	bool  const m_bOn;
};


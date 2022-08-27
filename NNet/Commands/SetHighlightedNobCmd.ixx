// SetHighlightedNobCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module SetHighlightedNobCmd;

import NobId;
import NNetCommand;

export class SetHighlightedNobCmd : public NNetCommand
{
public:
	SetHighlightedNobCmd(NobId const idNew)
	{
		m_idOld = m_pNMWI->GetHighlightedNobId();
		m_idNew = idNew;
	}

	void Do() final 
	{ 
		m_pNMWI->SetHighlightedNobId(m_idNew);
	}

	void Undo() final 
	{ 
		m_pNMWI->SetHighlightedNobId(m_idOld);
	}

private:

	NobId m_idNew;
	NobId m_idOld;
};
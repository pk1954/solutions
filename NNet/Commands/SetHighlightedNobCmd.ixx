// SetHighlightedNobCmd.ixx
//
// Commands

export module SetHighlightedNobCmd;

import NNetCommand;
import NNetModel;

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
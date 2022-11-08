// SelectSubtreeCommand.ixx
//
// Commands

export module SelectSubtreeCommand;

import SelectionCommand;
import NNetModel;

export class SelectSubtreeCommand : public SelectionCommand
{
public:
	SelectSubtreeCommand
	(
		NobId const id,
		bool  const bOn
	)
      : m_idNob(id),
		m_bOn(bOn)
	{ }

	void Do() final
	{
		SelectionCommand::Do();
		m_pNMWI->SelectSubtree(*m_pNMWI->GetNobPtr<PosNob*>(m_idNob), m_bOn);
	}

private:
	NobId const m_idNob;
	bool  const m_bOn;
};


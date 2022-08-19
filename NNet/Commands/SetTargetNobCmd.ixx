// SetTargetNobCmd.ixx
//
// Commands

module;

#include "NNetCommand.h"
#include "NobId.h"

export module SetTargetNobCmd;

export class SetTargetNobCmd : public NNetCommand
{
public:
	SetTargetNobCmd(NobId const id)
	{
		m_idNew = id;
		m_idOld = m_pNMWI->GetTargetNobId();
	}

	void Do() final
	{
		m_pNMWI->SetTargetNobId(m_idNew);
	}

	void Undo() final
	{
		m_pNMWI->SetTargetNobId(m_idOld);
	}

private:
	NobId m_idNew;
	NobId m_idOld;
};
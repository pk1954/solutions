// SetActiveSigGenCmd.ixx
//
// Commands

module;

#include "SigGenId.h"

export module SetActiveSigGenCmd;

import SigGenCommand;

export class SetActiveSigGenCmd : public SigGenCommand
{
public:
	SetActiveSigGenCmd(SigGenId const id)
	{
		m_sigGenIdNew = id;
	}

	void Do() final
	{
		m_sigGenIdOld = SetActiveSigGenId(m_sigGenIdNew);
	}

	void Undo() final
	{
		SetActiveSigGenId(m_sigGenIdOld);
	}

private:
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
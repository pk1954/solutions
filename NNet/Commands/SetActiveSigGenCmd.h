// SetActiveSigGenCmd.h
//
// Commands

#pragma once

#include "Resource.h"
#include "SignalGenerator.h"

import SigGenCommand;

class SetActiveSigGenCmd : public SigGenCommand
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
// SetActiveSigGenCmd.h
//
// Commands

#pragma once

#include "Resource.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class SetActiveSigGenCmd : public NNetCommand
{
public:
	SetActiveSigGenCmd(SigGenId const id)
	{
		m_sigGenIdNew = id;
	}

	void Do() final 
	{ 
		m_sigGenIdOld = m_pNMWI->SetSigGenActive(m_sigGenIdNew);
	}

	void Undo() final 
	{ 
		m_pNMWI->SetSigGenActive(m_sigGenIdOld);
	}

private:
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
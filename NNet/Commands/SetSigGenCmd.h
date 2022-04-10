// SetSigGenCmd.h
//
// Commands

#pragma once

#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class SetSigGenCmd : public NNetCommand
{
public:
	SetSigGenCmd(SigGenId const id)
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
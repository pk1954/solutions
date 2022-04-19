// SetSigGenCmd.h
//
// Commands

#pragma once

#include "Resource.h"
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
		PostCommand2Application(IDD_UNREGISTER_SIG_GEN, m_sigGenIdOld.GetValue());
		PostCommand2Application(IDD_REGISTER_SIG_GEN,   m_sigGenIdNew.GetValue());
	}

	void Undo() final 
	{ 
		m_pNMWI->SetSigGenActive(m_sigGenIdOld);
		PostCommand2Application(IDD_UNREGISTER_SIG_GEN, m_sigGenIdNew.GetValue());
		PostCommand2Application(IDD_REGISTER_SIG_GEN,   m_sigGenIdOld.GetValue());
	}

private:
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
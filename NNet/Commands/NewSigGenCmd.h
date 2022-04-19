// NewSigGenCmd.h
//
// Commands

#pragma once

#include "Resource.h"
#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class NewSigGenCmd : public NNetCommand
{
public:
	NewSigGenCmd()
	{
		m_upSigGen = m_pNMWI->NewSigGen();
	}

	void Do() final 
	{ 
		m_sigGenIdNew = m_pNMWI->PushSigGen(move(m_upSigGen));
		m_sigGenIdOld = m_pNMWI->SetSigGenActive(m_sigGenIdNew);
		PostCommand2Application(IDD_UNREGISTER_SIG_GEN, m_sigGenIdOld.GetValue());
		PostCommand2Application(IDD_REGISTER_SIG_GEN,   m_sigGenIdNew.GetValue());
	}

	void Undo() final 
	{ 
		m_pNMWI->SetSigGenActive(m_sigGenIdOld);
		PostCommand2Application(IDD_UNREGISTER_SIG_GEN, m_sigGenIdNew.GetValue());
		PostCommand2Application(IDD_REGISTER_SIG_GEN,   m_sigGenIdOld.GetValue());
		m_upSigGen = m_pNMWI->PopSigGen();
	}

private:
	UPSigGen m_upSigGen;
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
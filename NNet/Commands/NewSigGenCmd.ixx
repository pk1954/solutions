// NewSigGenCmd.ixx
//
// Commands

module;

#include "Resource.h"
#include "NNetModelWriterInterface.h"
#include "UPSigGenList.h"
#include "SignalGenerator.h"

export module NewSigGenCmd;

import Command;
import SigGenCommand;

export class NewSigGenCmd : public SigGenCommand
{
public:
	NewSigGenCmd()
	{
		m_upSigGen = m_pNMWI->NewSigGen();
	}

	void Do() final 
	{ 
		m_sigGenIdNew = m_pNMWI->PushSigGen(move(m_upSigGen));
		PostCommand2Application(IDD_REGISTER_SIG_GEN, m_sigGenIdNew.GetValue());
		SetActiveSigGenId(m_sigGenIdNew);
	}

	void Undo() final 
	{ 
		m_upSigGen = m_pNMWI->PopSigGen();
		SetActiveSigGenId(m_sigGenIdOld);
	}

private:
	UPSigGen m_upSigGen;
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
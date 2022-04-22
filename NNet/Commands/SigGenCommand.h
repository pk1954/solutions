// SigGenCommand.h
//
// Commands

#pragma once

#include "Resource.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

class SigGenCommand : public NNetCommand
{
public:
	SigGenId SetActiveSigGenId(SigGenId const idNew)
	{ 
		SigGenId sigGenIdOld = m_pNMWI->SetSigGenActive(idNew);
		PostCommand2Application(IDD_TRIGGER_SIGNAL_DESIGNER, 0);
		return sigGenIdOld;
	}
};
// SigGenCommand.ixx
//
// Commands

module;

#include "Resource.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

export module SigGenCommand;

export class SigGenCommand : public NNetCommand
{
public:
	SigGenId SetActiveSigGenId(SigGenId const idNew)
	{ 
		SigGenId sigGenIdOld = m_pNMWI->SetSigGenActive(idNew);
		PostCommand2Application(IDD_TRIGGER_SIGNAL_DESIGNER, 0);
		return sigGenIdOld;
	}
};
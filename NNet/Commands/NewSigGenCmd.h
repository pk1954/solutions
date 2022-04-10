// NewSigGenCmd.h
//
// Commands

#pragma once

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
	}

	void Undo() final 
	{ 
		m_pNMWI->SetSigGenActive(m_sigGenIdOld);
		m_upSigGen = m_pNMWI->PopSigGen();
	}

private:
	UPSigGen m_upSigGen;
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};
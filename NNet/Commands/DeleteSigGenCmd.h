// DeleteSigGenCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class DeleteSigGenCmd : public NNetCommand
{
public:
	DeleteSigGenCmd()
	{
		m_sigGenId = m_pNMWI->GetSigGenIdActive();
	}

	void Do() final 
	{
		// TODO: set all affected input lines to Standard
		m_upSigGen = move(m_pNMWI->RemoveSigGen(m_sigGenId));
	}

	void Undo() final 
	{
		m_pNMWI->InsertSigGen(move(m_upSigGen), m_sigGenId);
		// TODO: reset input lines
	}

private:
	SigGenId m_sigGenId;
	UPSigGen m_upSigGen;
};
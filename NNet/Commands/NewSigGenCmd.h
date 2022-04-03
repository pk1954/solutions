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
		m_pNMWI->PushSigGen(move(m_upSigGen));
	}

	void Undo() final 
	{ 
		m_upSigGen = m_pNMWI->PopSigGen();
	}

private:
	UPSigGen m_upSigGen;
};
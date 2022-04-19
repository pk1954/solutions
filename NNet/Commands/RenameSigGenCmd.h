// RenameSigGenCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class RenameSigGenCmd : public NNetCommand
{
public:
	RenameSigGenCmd()
	{
	}

	void Do() final 
	{ 
	}

	void Undo() final 
	{ 
	}

private:
	UPSigGen m_upSigGen;
};
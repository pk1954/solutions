// SetSigGenCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "NNetCommand.h"

using std::make_unique;

class SetSigGenCmd : public NNetCommand
{
public:
	SetSigGenCmd(SigGenId const id)
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
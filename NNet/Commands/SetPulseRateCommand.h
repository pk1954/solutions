// SetPulseRateCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "InputNeuron.h"

class SetPulseRateCommand : public Command
{
public:
	SetPulseRateCommand
	(
		NobId const id, 
		fHertz  const fNewValue 
	)
	  :	m_idInputNeuron(id),
		m_fHertz(fNewValue)
	{ }

	virtual void Do(NNetModelWriterInterface & nmwi)
	{ 
		m_fHertz = nmwi.GetNobPtr<InputNeuron *>(m_idInputNeuron)->SetPulseFrequency(m_fHertz);
		nmwi.ClearAllNobs();
	}

private:
	NobId const m_idInputNeuron;
	fHertz        m_fHertz;
};

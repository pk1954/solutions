// SetPulseRateCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "InputNeuron.h"

class SetPulseRateCommand : public NNetCommand
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

	virtual void Do()
	{ 
		m_fHertz = m_pNMWI->GetNobPtr<InputNeuron *>(m_idInputNeuron)->SetPulseFrequency(m_fHertz);
		m_pNMWI->ClearAllNobs();
	}

	virtual void Undo()
	{ 
		Do();
	}

private:
	NobId const m_idInputNeuron;
	fHertz        m_fHertz;
};

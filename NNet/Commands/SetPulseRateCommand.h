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
	SetPulseRateCommand( InputNeuron * pInputNeuron, fHertz const fNewValue )
	  :	m_pInputNeuron( pInputNeuron ),
		m_fHertz( fNewValue )
	{ }

	virtual void Do( NNetModelWriterInterface * const pModel )
	{ 
		m_fHertz = m_pInputNeuron->SetPulseFrequency( m_fHertz );
		pModel->ClearModel( );
	}

private:
	InputNeuron * const m_pInputNeuron;
	fHertz              m_fHertz;
};

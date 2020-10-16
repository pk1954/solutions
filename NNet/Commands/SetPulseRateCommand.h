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
		NNetModelWriterInterface & model, 
		ShapeId                    const id, 
		fHertz                     const fNewValue 
	)
	  :	m_pInputNeuron( model.GetShapePtr<InputNeuron *>(id) ),
		m_fHertz( fNewValue )
	{ }

	virtual void Do( NNetModelWriterInterface & model )
	{ 
		m_fHertz = m_pInputNeuron->SetPulseFrequency( m_fHertz );
		model.ClearModel( );
	}

private:
	InputNeuron * const m_pInputNeuron;
	fHertz              m_fHertz;
};

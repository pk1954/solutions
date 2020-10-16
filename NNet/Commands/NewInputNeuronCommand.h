// NewInputNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "InputNeuron.h"

class NewInputNeuronCommand : public Command
{
public:
	NewInputNeuronCommand
	( 
		NNetModelWriterInterface & model, 
		MicroMeterPoint    const & pos 
	)
	{ 
		m_pInputNeuron = model.NewBaseKnot<InputNeuron>( pos );
	}

	~NewInputNeuronCommand( )
	{ 
		delete m_pInputNeuron;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.Store2Model( m_pInputNeuron );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		model.RemoveFromModel( m_pInputNeuron );
	}

	InputNeuron * const GetInputNeuron( )
	{
		return m_pInputNeuron;
	}

private:
	InputNeuron * m_pInputNeuron;
};

// NewNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Neuron.h"

class NewNeuronCommand : public Command
{
public:
	NewNeuronCommand( NNetModelWriterInterface & model, MicroMeterPoint const & pos )
	{ 
		m_pNeuron = model.NewBaseKnot<Neuron>( pos );
	}

	~NewNeuronCommand( )
	{ 
		delete m_pNeuron;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		model.Store2Model( m_pNeuron );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		model.RemoveFromModel( m_pNeuron );
	}

	Neuron * const GetNeuron( )
	{
		return m_pNeuron;
	}

private:
	Neuron * m_pNeuron;
};


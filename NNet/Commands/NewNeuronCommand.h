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
	NewNeuronCommand( NNetModelWriterInterface * const pModel, MicroMeterPoint const & pos )
	{ 
		m_pNeuron = pModel->NewBaseKnot<Neuron>( pos );
	}

	~NewNeuronCommand( )
	{ 
		delete m_pNeuron;
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->Store2Model( m_pNeuron );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->RemoveFromModel( m_pNeuron );
	}

	Neuron * const GetNeuron( )
	{
		return m_pNeuron;
	}

private:
	Neuron * m_pNeuron;
};


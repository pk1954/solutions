// NewNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "Command.h"
#include "Neuron.h"

class NewNeuronCommand : public Command
{
public:
	NewNeuronCommand( NNetModel * const pModel, MicroMeterPoint const & pos )
	{ 
		m_pNeuron = pModel->NewBaseKnot<Neuron>( pos );
	}

	~NewNeuronCommand( )
	{ 
		delete m_pNeuron;
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->Store2Model( m_pNeuron );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		pModel->RemoveFromModel( m_pNeuron );
	}

private:
	Neuron * m_pNeuron;
};


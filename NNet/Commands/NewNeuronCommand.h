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
	NewNeuronCommand( MicroMeterPoint const & pos )
	{ 
		m_upNeuron = make_unique<Neuron>( pos );
	}

	~NewNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.GetShapes().Push( move(m_upNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upNeuron = nmwi.GetShapes().Pop<Neuron>( );
	}

private:
	unique_ptr<Neuron> m_upNeuron { nullptr };
};


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
	NewNeuronCommand( NNetModelWriterInterface & nmwi, MicroMeterPoint const & pos )
	{ 
		m_upNeuron = nmwi.NewBaseKnot<Neuron>( pos );
	}

	~NewNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Store2Model( move(m_upNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upNeuron = move(nmwi.RemoveFromModel<Neuron>( m_upNeuron->GetId() ));
	}

	ShapeId const GetNeuronId( )
	{
		return m_upNeuron.get()->GetId();
	}

private:
	unique_ptr<Neuron> m_upNeuron;
};


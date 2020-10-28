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
		NNetModelWriterInterface & nmwi, 
		MicroMeterPoint    const & pos 
	)
	{ 
		m_upInputNeuron = nmwi.NewBaseKnot<InputNeuron>( pos );
	}

	~NewInputNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Store2Model<Neuron>( move(m_upInputNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upInputNeuron = nmwi.RemoveFromModel<InputNeuron>( m_upInputNeuron->GetId() );
	}

	ShapeId const GetInputNeuronId( )
	{
		return m_upInputNeuron.get()->GetId();
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron;
};

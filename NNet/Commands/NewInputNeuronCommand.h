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
		m_idInputNeuron = nmwi.Add2Model( move(m_upInputNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upInputNeuron = nmwi.RemoveFromModel<InputNeuron>( m_idInputNeuron );
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron { nullptr };
	ShapeId                 m_idInputNeuron { NO_SHAPE };
};

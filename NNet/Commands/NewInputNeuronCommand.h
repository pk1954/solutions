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
	NewInputNeuronCommand( MicroMeterPoint const & pos )
	{ 
		m_upInputNeuron = make_unique<InputNeuron>( pos );
	}

	~NewInputNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Push2Model( move(m_upInputNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upInputNeuron = nmwi.PopFromModel<InputNeuron>( );
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron { nullptr };
};

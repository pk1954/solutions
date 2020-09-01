// NewInputNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "Command.h"
#include "InputNeuron.h"

class NewInputNeuronCommand : public Command
{
public:
	NewInputNeuronCommand
	( 
		NNetModelWriterInterface * const   pModel, 
		MicroMeterPoint            const & pos 
	)
	{ 
		m_pInputNeuron = pModel->NewBaseKnot<InputNeuron>( pos );
	}

	~NewInputNeuronCommand( )
	{ 
		delete m_pInputNeuron;
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->Store2Model( m_pInputNeuron );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		pModel->RemoveFromModel( m_pInputNeuron );
	}

	InputNeuron * const GetInputNeuron( )
	{
		return m_pInputNeuron;
	}

private:
	InputNeuron * m_pInputNeuron;
};

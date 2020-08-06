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
	NewInputNeuronCommand( NNetModel * const pModel, MicroMeterPoint const & pos )
	{ 
		m_pInputNeuron = pModel->NewBaseKnot<InputNeuron>( pos );
	}

	~NewInputNeuronCommand( )
	{ 
		delete m_pInputNeuron;
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->Store2Model( m_pInputNeuron );
	}

	virtual void Undo( NNetModel * const pModel ) 
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

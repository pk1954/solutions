// Convert2NeuronCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "InputNeuron.h"
#include "Neuron.h"

class Convert2NeuronCommand : public Command
{
public:
	Convert2NeuronCommand(  NNetModel * pModel, ShapeId const idInputNeuron )
		:	m_pInputNeuron( pModel->GetShapePtr<InputNeuron *>( idInputNeuron )  )
	{ 
		m_pNeuron = pModel->NewBaseKnot<Neuron>( m_pInputNeuron->GetPosition( ) );
		m_pNeuron->m_connections = m_pInputNeuron->m_connections;
	}

	~Convert2NeuronCommand( )
	{
		delete m_pNeuron;
	}

	virtual void Do( NNetModel * const pModel )
	{
		pModel->ReplaceInModel( m_pInputNeuron, m_pNeuron );
	}

	virtual void Undo( NNetModel * const pModel )
	{
		pModel->ReplaceInModel( m_pNeuron, m_pInputNeuron );
	}

private:
	InputNeuron * m_pInputNeuron;
	Neuron      * m_pNeuron;
};

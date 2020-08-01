// Convert2InputNeuronCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "Neuron.h"
#include "InputNeuron.h"

class Convert2InputNeuronCommand : public Command
{
public:
	Convert2InputNeuronCommand( NNetModel * pModel, ShapeId const idNeuron )
		:	m_pNeuron( pModel->GetShapePtr<Neuron *>( idNeuron )  )
	{ 
		m_pInputNeuron = pModel->NewBaseKnot<InputNeuron>( m_pNeuron->GetPosition( ) );
		m_pInputNeuron->m_connections = m_pNeuron->m_connections;
	}

	~Convert2InputNeuronCommand( )
	{
		delete m_pInputNeuron;
	}

	virtual void Do( NNetModel * const pModel )
	{
		pModel->ReplaceInModel( m_pNeuron, m_pInputNeuron );
	}

	virtual void Undo( NNetModel * const pModel )
	{
		pModel->ReplaceInModel( m_pInputNeuron, m_pNeuron );
	}

private:
	Neuron      * m_pNeuron;
	InputNeuron * m_pInputNeuron;
};

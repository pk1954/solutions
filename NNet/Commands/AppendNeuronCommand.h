// AppendNeuronCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "Neuron.h"
#include "Knot.h"

class AppendNeuronCommand : public Command
{
public:
	AppendNeuronCommand( NNetModel * pModel, ShapeId const id )
	{ 
		m_pKnot   = pModel->GetShapePtr<Knot *>( id );
		m_pNeuron = pModel->NewBaseKnot<Neuron>( m_pKnot->GetPosition( ) );
		m_pNeuron->m_connections = m_pKnot->m_connections;
	}

	~AppendNeuronCommand( )
	{
		delete m_pNeuron;
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->ReplaceInModel( m_pKnot, m_pNeuron );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		pModel->ReplaceInModel( m_pNeuron, m_pKnot );
	}

private:
	Neuron * m_pNeuron;
	Knot   * m_pKnot;
};

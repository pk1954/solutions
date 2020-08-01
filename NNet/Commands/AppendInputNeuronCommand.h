// AppendInputNeuronCommand.h
//
// Commands

#pragma once

#include "NNetModel.h"
#include "ShapeId.h"
#include "Command.h"
#include "Knot.h"

class AppendInputNeuronCommand : public Command
{
public:
	AppendInputNeuronCommand( NNetModel * pModel, ShapeId const id )
	{ 
		m_pKnot        = pModel->GetShapePtr<Knot *>( id );
		m_pInputNeuron = new InputNeuron( m_pKnot->GetPosition( ) );
		m_pInputNeuron->m_connections = m_pKnot->m_connections;
	}

	~AppendInputNeuronCommand( )
	{
		delete m_pInputNeuron;
	}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		pModel->ReplaceInModel( m_pKnot, m_pInputNeuron );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		pModel->ReplaceInModel( m_pInputNeuron, m_pKnot );
	}

private:
	InputNeuron * m_pInputNeuron;
	Knot        * m_pKnot;
};

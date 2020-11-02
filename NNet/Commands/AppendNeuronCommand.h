// AppendNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "Neuron.h"

class AppendNeuronCommand : public Command
{
public:
	AppendNeuronCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId              const idKnot
	)
	  : m_idKnot( idKnot )
	{     
		Knot const * pKnot { nmwi.GetShapePtr<Knot *>(idKnot) };
		m_upNeuron = make_unique<Neuron>( pKnot->GetPosition() );
		m_upNeuron->m_connections = pKnot->m_connections;
		m_upNeuron->SetId( idKnot );
	}

	~AppendNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upNeuron->Reconnect( );
		m_upKnot = nmwi.RemoveFromModel<Knot>( m_idKnot ); 
		nmwi.Store2Model( move(m_upNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnot->Reconnect( );
		m_upNeuron = nmwi.RemoveFromModel<Neuron>( m_idKnot );
		nmwi.Store2Model( move( m_upKnot ) ); 
	}

private:
	unique_ptr<Neuron> m_upNeuron { nullptr };
	unique_ptr<Knot>   m_upKnot   { nullptr };
	ShapeId            m_idKnot   { NO_SHAPE };
};

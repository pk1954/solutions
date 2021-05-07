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
	AppendNeuronCommand( NobId const idKnot )
	  : m_idKnot( idKnot )
	{}

	~AppendNeuronCommand() {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_upNeuron )
		{     
			Knot const * pKnot { nmwi.GetNobPtr<Knot *>(m_idKnot) };
			m_upNeuron = make_unique<Neuron>( pKnot->GetPos() );
			m_upNeuron->m_connections = pKnot->m_connections;
			m_upNeuron->SetId( m_idKnot );
		}
		m_upNeuron->Reconnect();
		m_upKnot = nmwi.ReplaceInModel<Neuron,Knot>( move(m_upNeuron) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnot->Reconnect();
		m_upNeuron = nmwi.ReplaceInModel<Knot,Neuron>( move( m_upKnot ) ); 
	}

private:
	unique_ptr<Neuron> m_upNeuron { nullptr };
	unique_ptr<Knot>   m_upKnot   { nullptr };
	NobId      const m_idKnot;
};

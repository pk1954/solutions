// AppendInputNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "InputNeuron.h"

class AppendInputNeuronCommand : public Command
{
public:
	AppendInputNeuronCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId              const idKnot
	)
		: m_idKnot( idKnot )
	{     
		Knot const * pKnot { nmwi.GetShapePtr<Knot *>(idKnot) };
		m_upInputNeuron = make_unique<InputNeuron>( pKnot->GetPosition() );
		m_upInputNeuron->m_connections = pKnot->m_connections;
		m_upInputNeuron->SetId( idKnot );
	}

	~AppendInputNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upInputNeuron->Reconnect( );
		m_upKnot = nmwi.RemoveFromModel<Knot>( m_idKnot ); 
		nmwi.Store2Model( move(m_upInputNeuron) ); 
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnot->Reconnect( );
		m_upInputNeuron = nmwi.RemoveFromModel<InputNeuron>( m_idKnot );
		nmwi.Store2Model( move( m_upKnot ) ); 
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron { nullptr };
	unique_ptr<Knot>        m_upKnot        { nullptr };
	ShapeId                 m_idKnot        { NO_SHAPE };
	ShapeId                 m_idInputNeuron { NO_SHAPE };
};

// AppendInputNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "InputNeuron.h"

class AppendInputNeuronCommand : public Command
{
public:
	AppendInputNeuronCommand( ShapeId const idKnot )
		: m_idKnot( idKnot )
	{ }

	~AppendInputNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_bInitialized )
		{
			Knot * pKnot { nmwi.GetShapePtr<Knot *>(m_idKnot) };
			m_upInputNeuron = make_unique<InputNeuron>( pKnot->GetPosition() );
			m_upInputNeuron->m_connections = pKnot->m_connections;
			m_upInputNeuron->SetId( m_idKnot );
			m_bInitialized = true;
		}
		m_upInputNeuron->Reconnect( );
		m_upKnot = nmwi.ReplaceInModel<InputNeuron, Knot>( move(m_upInputNeuron) ); 
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnot->Reconnect( );
		m_upInputNeuron = nmwi.ReplaceInModel<Knot, InputNeuron>( move( m_upKnot ) ); 
	}

private:
	unique_ptr<InputNeuron> m_upInputNeuron { nullptr };
	unique_ptr<Knot>        m_upKnot        { nullptr };
	ShapeId                 m_idKnot        { NO_SHAPE };
	bool                    m_bInitialized  { false };
};

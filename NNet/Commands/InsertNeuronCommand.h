// InsertNeuronCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class InsertNeuronCommand : public Command
{
public:
	InsertNeuronCommand
	( 
		NNetModelWriterInterface & model,
		ShapeId            const   id, 
		MicroMeterPoint    const & umSplitPoint 
	)
	{ 
		m_pPipe2Split = model.GetShapePtr<Pipe *>( id );
		m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr( );
		m_pNeuron     = model.NewBaseKnot<Neuron>( umSplitPoint );
		m_pPipeNew    = model.NewPipe( m_pStartKnot, m_pNeuron );
		m_pNeuron->m_connections.AddOutgoing( m_pPipe2Split );
		m_pNeuron->m_connections.AddIncoming( m_pPipeNew );
	}

	~InsertNeuronCommand( )
	{
		delete m_pNeuron;
		delete m_pPipeNew;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe2Split, m_pPipeNew );
		m_pPipe2Split->SetStartKnot( m_pNeuron );
		model.Store2Model( m_pNeuron );
		model.Store2Model( m_pPipeNew );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		model.RemoveFromModel( m_pNeuron );
		model.RemoveFromModel( m_pPipeNew );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipeNew, m_pPipe2Split );
		m_pPipe2Split->SetStartKnot( m_pStartKnot );
	}

private:
	Pipe     * m_pPipe2Split { nullptr };
	Pipe     * m_pPipeNew    { nullptr };
	Neuron   * m_pNeuron     { nullptr };
	BaseKnot * m_pStartKnot  { nullptr };
};

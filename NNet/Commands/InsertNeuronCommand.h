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
		NNetModelWriterInterface & nmwi,
		ShapeId            const   id, 
		MicroMeterPoint    const & umSplitPoint 
	)
	{ 
		m_pPipe2Split = nmwi.GetShapePtr<Pipe *>( id );
		m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr( );
		m_upNeuron    = nmwi.NewBaseKnot<Neuron>( umSplitPoint );
		m_upPipeNew   = nmwi.NewPipe( m_pStartKnot, m_upNeuron.get() );
		m_upNeuron->m_connections.AddOutgoing( m_pPipe2Split );
		m_upNeuron->m_connections.AddIncoming( m_upPipeNew.get() );
	}

	~InsertNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe2Split, m_upPipeNew.get() );
		m_pPipe2Split->SetStartKnot( m_upNeuron.get() );
		nmwi.Store2Model<Neuron>( move(m_upNeuron) );
		nmwi.Store2Model<Pipe>  ( move(m_upPipeNew) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upNeuron  = move( nmwi.RemoveFromModel<Neuron>( m_upNeuron ->GetId() ));
		m_upPipeNew = move( nmwi.RemoveFromModel<Pipe>  ( m_upPipeNew->GetId() ));
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upPipeNew.get(), m_pPipe2Split );
		m_pPipe2Split->SetStartKnot( m_pStartKnot );
	}

private:
	Pipe     * m_pPipe2Split { nullptr };
	BaseKnot * m_pStartKnot  { nullptr };

	unique_ptr<Pipe>   m_upPipeNew { nullptr };
	unique_ptr<Neuron> m_upNeuron  { nullptr };
};

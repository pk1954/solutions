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
		ShapeId         const   idPipe, 
		MicroMeterPoint const & umSplitPoint 
	)
	  :	m_idPipe(idPipe),
		m_umSplitPoint(umSplitPoint)
	{ }

	~InsertNeuronCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_upNeuron )
		{ 
			m_pPipe2Split = nmwi.GetShapePtr<Pipe *>( m_idPipe );
			m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr( );
			m_upNeuron    = make_unique<Neuron>( m_umSplitPoint );
			m_upPipeNew   = make_unique<Pipe>( m_pStartKnot, m_upNeuron.get() );
			m_upNeuron->m_connections.AddOutgoing( m_pPipe2Split );
			m_upNeuron->m_connections.AddIncoming( m_upPipeNew.get() );
		}
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe2Split, m_upPipeNew.get() );
		m_pPipe2Split->SetStartKnot( m_upNeuron.get() );
		nmwi.Push2Model( move(m_upNeuron) );
		nmwi.Push2Model( move(m_upPipeNew) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upPipeNew = nmwi.PopFromModel<Pipe>  ();
		m_upNeuron  = nmwi.PopFromModel<Neuron>();
		m_pPipe2Split->SetStartKnot( m_pStartKnot );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upPipeNew.get(), m_pPipe2Split );
	}

private:
	Pipe             * m_pPipe2Split { nullptr };
	BaseKnot         * m_pStartKnot  { nullptr };
	unique_ptr<Pipe>   m_upPipeNew   { nullptr };
	unique_ptr<Neuron> m_upNeuron    { nullptr };
	ShapeId         const m_idPipe;
	MicroMeterPoint const m_umSplitPoint; 
};

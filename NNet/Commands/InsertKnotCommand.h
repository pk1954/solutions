// InsertKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class InsertKnotCommand : public Command
{
public:
	InsertKnotCommand
	( 
		ShapeId         const   idPipe, 
		MicroMeterPoint const & umSplitPoint 
	)
	  :	m_idPipe(idPipe),
		m_umSplitPoint(umSplitPoint)
	{ }

	~InsertKnotCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_upKnot )
		{ 
			m_pPipe2Split = nmwi.GetShapePtr<Pipe *>( m_idPipe );
			m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr( );
			m_upKnot      = make_unique<Knot>( m_umSplitPoint );
			m_upPipeNew   = make_unique<Pipe>( m_pStartKnot, m_upKnot.get() );
			m_upKnot->m_connections.AddOutgoing( m_pPipe2Split );
			m_upKnot->m_connections.AddIncoming( m_upPipeNew.get() );
		}
		m_pStartKnot->m_connections.ReplaceOutgoing( m_pPipe2Split, m_upPipeNew.get() );
		m_pPipe2Split->SetStartKnot( m_upKnot.get() );
		nmwi.Push2Model( move(m_upKnot) );
		nmwi.Push2Model( move(m_upPipeNew) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upPipeNew = nmwi.PopFromModel<Pipe>();
		m_upKnot    = nmwi.PopFromModel<Knot>();
		m_pPipe2Split->SetStartKnot( m_pStartKnot );
		m_pStartKnot->m_connections.ReplaceOutgoing( m_upPipeNew.get(), m_pPipe2Split );
	}

private:
	Pipe             * m_pPipe2Split { nullptr };
	BaseKnot         * m_pStartKnot  { nullptr };
	unique_ptr<Pipe>   m_upPipeNew   { nullptr };
	unique_ptr<Knot>   m_upKnot      { nullptr };
	ShapeId         const m_idPipe;
	MicroMeterPoint const m_umSplitPoint; 
};

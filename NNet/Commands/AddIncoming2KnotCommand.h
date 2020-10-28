// AddIncoming2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"

class AddIncoming2KnotCommand : public Command
{
public:
	AddIncoming2KnotCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId            const   id, 
		MicroMeterPoint    const & pos 
	)
	  : m_pEnd( nmwi.GetShapePtr<BaseKnot *>( id ) )
	{ 		
		m_upKnotNew = nmwi.NewBaseKnot<Knot>( pos );
		m_upPipe    = nmwi.NewPipe( m_upKnotNew.get(), m_pEnd );
		m_upKnotNew->m_connections.AddOutgoing( m_upPipe.get() );
	}

	~AddIncoming2KnotCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pEnd->m_connections.AddIncoming( m_upPipe.get() );
		m_idKnotNew = nmwi.Add2Model( move(m_upKnotNew) );
		m_idPipe    = nmwi.Add2Model( move(m_upPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_upKnotNew = nmwi.RemoveFromModel<Knot>( m_idKnotNew );
		m_upPipe    = nmwi.RemoveFromModel<Pipe>( m_idPipe );
		m_pEnd->m_connections.RemoveIncoming( m_upPipe.get() );
	}

private:
	BaseKnot * const m_pEnd      { nullptr };
	unique_ptr<Knot> m_upKnotNew { nullptr };
	unique_ptr<Pipe> m_upPipe    { nullptr };
	ShapeId          m_idKnotNew { NO_SHAPE };
	ShapeId          m_idPipe    { NO_SHAPE };
};


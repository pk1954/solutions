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
		nmwi.Store2Model<Knot>( move(m_upKnotNew) );
		nmwi.Store2Model<Pipe>( move(m_upPipe) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pEnd->m_connections.RemoveIncoming( m_upPipe.get() );
		
		m_upKnotNew = move( nmwi.RemoveFromModel<Knot>( m_upKnotNew->GetId() ) );
		m_upPipe    = move( nmwi.RemoveFromModel<Pipe>( m_upPipe->GetId() ) );
	}

private:
	BaseKnot * const m_pEnd;
	unique_ptr<Knot> m_upKnotNew;
	unique_ptr<Pipe> m_upPipe;
};


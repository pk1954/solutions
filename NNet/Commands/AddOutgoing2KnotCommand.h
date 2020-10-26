// AddOutgoing2KnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

class AddOutgoing2KnotCommand : public Command
{
public:
	AddOutgoing2KnotCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId            const   id, 
		MicroMeterPoint    const & pos 
	)
	{ 
		m_pStart    = nmwi.GetShapePtr<BaseKnot *>( id );
		m_upKnotNew = nmwi.NewBaseKnot<Knot>( pos );
		m_upPipe    = nmwi.NewPipe( m_pStart, m_upKnotNew.get() );
		m_upKnotNew->m_connections.AddIncoming( m_upPipe.get() );
	}

	~AddOutgoing2KnotCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStart->m_connections.AddOutgoing( m_upPipe.get() );
		nmwi.Store2Model( move( m_upKnotNew ) );
		nmwi.Store2Model( move( m_upPipe ) );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_pStart->m_connections.RemoveOutgoing( m_upPipe.get() );
		m_upKnotNew = move(nmwi.RemoveFromModel<Knot>( m_upKnotNew->GetId() ) );
		m_upPipe    = move(nmwi.RemoveFromModel<Pipe>( m_upPipe->GetId() ) );
	}

private:
	BaseKnot * m_pStart;
	unique_ptr<Knot> m_upKnotNew;
	unique_ptr<Pipe> m_upPipe;
};

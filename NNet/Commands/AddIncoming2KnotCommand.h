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
		NNetModelWriterInterface & model, 
		ShapeId            const   id, 
		MicroMeterPoint    const & pos 
	)
	  : m_pEnd( model.GetShapePtr<BaseKnot *>( id ) )
	{ 		
		m_pKnotNew = model.NewBaseKnot<Knot>( pos );
		m_pPipe    = model.NewPipe( m_pKnotNew, m_pEnd );
		m_pKnotNew->m_connections.AddOutgoing( m_pPipe );
	}

	~AddIncoming2KnotCommand( )
	{
		delete m_pKnotNew;
		delete m_pPipe;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pEnd->m_connections.AddIncoming( m_pPipe );
		model.Store2Model( m_pKnotNew );
		model.Store2Model( m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pEnd->m_connections.RemoveIncoming( m_pPipe );
		model.RemoveFromModel( m_pKnotNew );
		model.RemoveFromModel( m_pPipe );
	}

private:
	BaseKnot * const m_pEnd;
	Knot     *       m_pKnotNew;
	Pipe     *       m_pPipe;
};


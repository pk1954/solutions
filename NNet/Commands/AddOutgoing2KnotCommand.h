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
		NNetModelWriterInterface & model, 
		ShapeId            const   id, 
		MicroMeterPoint    const & pos 
	)
	{ 
		m_pStart   = model.GetShapePtr<BaseKnot *>( id );
		m_pKnotNew = model.NewBaseKnot<Knot>( pos );
		m_pPipe    = model.NewPipe( m_pStart, m_pKnotNew );
		m_pKnotNew->m_connections.AddIncoming( m_pPipe );
	}

	~AddOutgoing2KnotCommand( )
	{
		delete m_pKnotNew;
		delete m_pPipe;
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pStart->m_connections.AddOutgoing( m_pPipe );
		model.Store2Model( m_pKnotNew );
		model.Store2Model( m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pStart->m_connections.RemoveOutgoing( m_pPipe );
		model.RemoveFromModel( m_pKnotNew );
		model.RemoveFromModel( m_pPipe );
	}

private:
	Knot     * m_pKnotNew;
	BaseKnot * m_pStart;
	Pipe     * m_pPipe;
};

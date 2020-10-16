// MoveBaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MoveCommand.h"
#include "BaseKnot.h"

class MoveBaseKnotCommand : public MoveCommand
{
public:
	MoveBaseKnotCommand
	( 
		NNetModelWriterInterface & model, 
		ShapeId            const   id, 
		MicroMeterPoint    const & delta 
	)
	  : MoveCommand( delta ),
		m_pBaseKnot( model.GetShapePtr<BaseKnot *>( id ) )
	{
		m_posBaseKnotOld = m_pBaseKnot->GetPosition();
	}

	virtual void Do( NNetModelWriterInterface & model ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
	}

	virtual void Undo( NNetModelWriterInterface & model ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld );
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_pBaseKnot->GetId( );
	}

private:
	BaseKnot * const m_pBaseKnot;
	MicroMeterPoint  m_posBaseKnotOld { NP_NULL };
};

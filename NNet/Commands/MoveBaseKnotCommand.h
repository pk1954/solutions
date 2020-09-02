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
	MoveBaseKnotCommand( BaseKnot * const pBaseKnot, MicroMeterPoint const & delta )
	  : MoveCommand     ( delta ),
		m_pBaseKnot     ( pBaseKnot ),
		m_posBaseKnotOld( pBaseKnot->GetPosition() )
	{}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld );
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return m_pBaseKnot->GetId( );
	}

private:
	BaseKnot      * const m_pBaseKnot;
	MicroMeterPoint const m_posBaseKnotOld;
};

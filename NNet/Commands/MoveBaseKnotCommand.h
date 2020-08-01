// MoveBaseKnotCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "Command.h"
#include "BaseKnot.h"

class MoveBaseKnotCommand : public Command
{
public:
	MoveBaseKnotCommand( BaseKnot * const pBaseKnot, MicroMeterPoint const & delta )
		: m_pBaseKnot     ( pBaseKnot ),
		m_delta         ( delta ),
		m_posBaseKnotOld( pBaseKnot->GetPosition() )
	{}

	virtual void Do( NNetModel * const pModel ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{ 
		m_pBaseKnot->SetPosition( m_posBaseKnotOld );
	}

private:
	BaseKnot      * const m_pBaseKnot;
	MicroMeterPoint const m_delta;
	MicroMeterPoint const m_posBaseKnotOld;
};

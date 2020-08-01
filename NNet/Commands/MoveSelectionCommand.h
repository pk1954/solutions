// MoveSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModel.h"
#include "Command.h"
#include "BaseKnot.h"

class MoveSelectionCommand : public Command
{
public:
	MoveSelectionCommand( MicroMeterPoint const & delta )
		: m_delta( -delta )
	{ }

	virtual void Do( NNetModel * const pModel ) 
	{ 
		MicroMeterPoint const delta = - m_delta;
		pModel->Apply2AllSelected<BaseKnot>
			( 
				[&]( BaseKnot & knot ) 
				{ 
					knot.MoveShape( delta ); 
					knot.m_connections.Apply2AllConnectedPipes( [&](Pipe & pipe) { pipe.Recalc(); } );
				} 
		);
		m_delta = delta;
	}

private:
	MicroMeterPoint m_delta;
};

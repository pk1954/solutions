// MoveSelectionCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "BaseKnot.h"

class MoveSelectionCommand : public Command
{
public:
	MoveSelectionCommand( MicroMeterPoint const & delta )
       : m_delta( delta )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		( 
			[&](Nob & nob) 
			{ 
				if (! nob.IsPipe())
					nob.MoveNob(m_delta);
			} 
		);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		( 
			[&](Nob & nob) 
			{ 
				if (! nob.IsPipe())
					nob.MoveNob(-m_delta);
			} 
		);
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

	virtual bool const Combine(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		MoveSelectionCommand const & srcCmd { static_cast<MoveSelectionCommand const &>(src) };
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPoint m_delta;
};

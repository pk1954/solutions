// MoveNobCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "NobId.h"
#include "Nob.h"

class MoveNobCommand : public Command
{
public:
	MoveNobCommand
	( 
		Nob                   & nob, 
		MicroMeterPoint const & delta 
	)
	  : m_delta( delta ),
		m_nob( nob )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		m_nob.MoveNob(m_delta);
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		m_nob.MoveNob(-m_delta);
	}

	virtual NobId const GetMovedNob() const
	{
		return m_nob.GetId();
	}

	virtual bool IsMoveCommand() const
	{ 
		return true; 
	};

	virtual bool const Combine(Command const & src) 
	{ 
		if (typeid(src) != typeid(*this))
			return false;
		MoveNobCommand const & srcCmd { static_cast<MoveNobCommand const &>(src) };
		if (m_nob.GetId() != srcCmd.m_nob.GetId())
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPoint m_delta;
	Nob           & m_nob;
};

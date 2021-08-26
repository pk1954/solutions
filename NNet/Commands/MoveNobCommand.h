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
		Nob                 & nob, 
		MicroMeterPnt const & delta 
	)
	  : m_delta(delta),
		m_nob(nob)
	{ 
		assert(&nob);
	}

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.MoveNob(m_delta);
	}

	virtual void Undo(NNetModelWriterInterface & nmwi) 
	{ 
		m_nob.MoveNob(-m_delta);
	}

	virtual NobId const GetAffectedNob() const { return m_nob.GetId(); }

	virtual bool const CombineCommands(Command const & src) 
	{ 
		MoveNobCommand const & srcCmd { static_cast<MoveNobCommand const &>(src) };
		if (GetAffectedNob() != srcCmd.GetAffectedNob())
			return false;
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPnt m_delta;
	Nob           & m_nob;
};

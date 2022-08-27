// MoveNobCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "Nob.h"

export module MoveNobCommand;

import MoreTypes;
import NobId;
import NNetCommand;

export class MoveNobCommand : public NNetCommand
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

	void Do() final 
	{ 
		m_nob.MoveNob(m_delta);
	}

	void Undo() final
	{ 
		m_nob.MoveNob(-m_delta);
	}

	NobId GetAffectedNob() const final { return m_nob.GetId(); }

	bool CombineCommands(Command const & src) final
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

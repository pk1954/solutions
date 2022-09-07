// MoveSelectionCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "BaseKnot.h"

export module MoveSelectionCommand;

import Types;
import NNetCommand;

export class MoveSelectionCommand : public NNetCommand
{
public:
	explicit MoveSelectionCommand(MicroMeterPnt const & delta)
		: m_delta(delta)
	{ }

	void Do() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2AllSelected<BaseKnot>
			(
				[this](Nob & nob) { nob.MoveNob(m_delta); } 
		);
	}

	void Undo() final 
	{ 
		m_pNMWI->GetUPNobs().Apply2AllSelected<BaseKnot>
			(
				[this](Nob & nob) { nob.MoveNob(-m_delta); } 
		);
	}

	bool CombineCommands(Command const & src) final
	{ 
		MoveSelectionCommand const & srcCmd { static_cast<MoveSelectionCommand const &>(src) };
		m_delta += srcCmd.m_delta;
		return true; 
	};

private:
	MicroMeterPnt m_delta;
};

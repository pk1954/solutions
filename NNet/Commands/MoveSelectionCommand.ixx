// MoveSelectionCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:MoveSelectionCommand;

import :NNetCommand;

export class MoveSelectionCommand : public NNetCommand
{
public:
	explicit MoveSelectionCommand(MicroMeterPnt const & delta)
		: m_delta(delta)
	{ }

	void Do() final 
	{ 
		moveSelected(m_delta);
	}

	void Undo() final 
	{ 
		moveSelected(-m_delta);
	}

	void moveSelected(MicroMeterPnt const & delta)
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>
		(
			[delta](PosNob& posNob)	{ posNob.MoveNob(delta); }
		);
	}

	bool CombineCommands(Command const & src) final
	{ 
		MoveSelectionCommand const & srcCmd { static_cast<MoveSelectionCommand const &>(src) };
		m_delta += srcCmd.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << delta << endl;
		m_pStack->PushCommand(make_unique<MoveSelectionCommand>(delta));
	}

private:

	inline static const wstring NAME { L"MoveSelection" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			MoveSelectionCommand::Push(ScrReadMicroMeterPnt(script));
		}
	} m_wrapper;

	MicroMeterPnt m_delta;
};

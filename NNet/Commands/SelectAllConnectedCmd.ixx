// SelectAllConnectedCmd.ixx
//
// Commands

module;

#include <iostream>

export module SelectAllConnectedCmd;

import SelectionCommand;
import UtilityWrappers;

export class SelectAllConnectedCmd : public SelectionCommand
{
public:
	SelectAllConnectedCmd(NobId const idNob, tBoolOp const bOp)
	  : m_nobStart(*m_pNMWI->GetNobPtr<Nob *>(idNob)),
		m_bOn(ApplyOp2(m_nobStart.IsSelected(), bOp))
	{}

	void Do() final
	{
		SelectionCommand::Do();
		m_nobStart.SelectAllConnected(true, m_bOn);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId const idNob, tBoolOp const bOp)
	{
		if (IsTraceOn())
			TraceStream() << NAME << L' ' << idNob << L' ' << bOp << L' ' << endl;
		m_pStack->PushCommand(make_unique<SelectAllConnectedCmd>(idNob, bOp));
	}

private:

	inline static const wstring NAME { L"SelectAllConnected" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId   const idSrc { ScrReadNobId(script) };
			tBoolOp const bOp   { ScrReadBoolOp(script) };
			SelectAllConnectedCmd::Push(idSrc, bOp);
		}
	};

	Nob& m_nobStart;
	bool m_bOn;
};

// SelectAllConnectedCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:SelectAllConnectedCmd;

import :SelectionCommand;
import IoUtil;

export class SelectAllConnectedCmd : public SelectionCommand
{
public:
	SelectAllConnectedCmd(NobId const idNob)
	  : m_nobStart(*m_pNMWI->GetNobPtr<Nob *>(idNob))
	{}

	void Do() final
	{
		SelectionCommand::Do();    // not needed?
		m_nobStart.SelectAllConnected(true);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const idNob)
	{
		if (IsTraceOn())
			TraceStream() << NAME << SPACE << idNob << endl;
		PushCommand(make_unique<SelectAllConnectedCmd>(idNob));
	}

private:

	inline static const wstring NAME { L"SelectAllConnected" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			SelectAllConnectedCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper;

	Nob& m_nobStart;
	bool m_bOn;
};

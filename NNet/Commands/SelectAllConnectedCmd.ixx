// SelectAllConnectedCmd.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module SelectAllConnectedCmd;

import Symtab;
import Script;
import SelectionCommand;
import NNetModel;

using std::endl;
using std::wstring;
using std::make_unique;

export class SelectAllConnectedCmd : public SelectionCommand
{
public:
	SelectAllConnectedCmd()
	{ }

	void Do() final
	{
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>([](Nob& n){ n.SelectAllConnected(true); });
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pStack->PushCommand(make_unique<SelectAllConnectedCmd>());
	}

private:

	inline static const wstring NAME { L"SelectAllConnected" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SelectAllConnectedCmd::Push();
		}
	};

	bool m_bOn;
};

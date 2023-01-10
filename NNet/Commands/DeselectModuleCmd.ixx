// DeselectModuleCmd.ixx
//
// Commands

module;

#include <iostream>

export module DeselectModuleCmd;

import SelectionCommand;
import NNetModel;

export class DeselectModuleCmd : public SelectionCommand
{
public:
	DeselectModuleCmd()
	{ }

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().DeselectAllNobs();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pStack->PushCommand(make_unique<DeselectModuleCmd>());
	}

private:

	inline static const wstring NAME { L"DeselectModule" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			DeselectModuleCmd::Push();
		}
	};
};

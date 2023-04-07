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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pStack->PushCommand(make_unique<DeselectModuleCmd>());
	}

private:

	inline static const wstring NAME { L"DeselectModule" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			DeselectModuleCmd::Push();
		}
	} m_wrapper;
};

// ToggleScanModeCmd.ixx
//
// NNetCommands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:ToggleScanModeCmd;

import :NNetCommand;
import NNetModel;
import IoUtil;

export class ToggleScanModeCmd : public NNetCommand
{
public:
	explicit ToggleScanModeCmd()
	{}

	void Do() final
	{
		m_pNMWI->ToggleScanMode();
	}

	void Undo() final
	{
		Do();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		PushCommand(make_unique<ToggleScanModeCmd>());
	}

private:

	inline static const wstring NAME { L"ToggleScanMode" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			ToggleScanModeCmd::Push();
		}
	} m_wrapper { NAME };
};

// RedoCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:RedoCommand;

import :NNetCommand;

export class RedoCommand : public NNetCommand
{
public:

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		if (!StdStackCommand::RedoCommand())
			PlayWarningSound();
	}

private:

	inline static const wstring NAME { L"Redo" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			RedoCommand::Push();
		}
	} m_wrapper { NAME };

};

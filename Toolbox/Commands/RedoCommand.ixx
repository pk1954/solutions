// RedoCommand.ixx
//
// Toolbox\Commands

module;

#include <iostream>
#include <string>
#include <iostream>

export module RedoCommand;

import StdStackCommand;

export class RedoCommand : public StdStackCommand
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

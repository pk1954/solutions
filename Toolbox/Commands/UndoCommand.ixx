// UndoCommand.ixx
//
// Toolbox\Commands

module;

#include <iostream>
#include <string>
#include <iostream>

export module Commands:UndoCommand;

import :StdStackCommand;
import :Wrapper;

export class UndoCommand : public StdStackCommand
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
		if (!StdStackCommand::UndoCommand())
			PlayWarningSound();
	}

private:

	inline static const wstring NAME { L"Undo" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			UndoCommand::Push();
		}
	} m_wrapper { NAME };

};

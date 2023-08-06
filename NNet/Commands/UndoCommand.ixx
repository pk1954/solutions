// UndoCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:UndoCommand;

import :NNetCommand;

export class UndoCommand : public NNetCommand
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
		if (!Command::UndoCommand())
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

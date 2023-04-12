// UndoCommand.ixx
//
// Commands

module;

#include <iostream>

export module UndoCommand;

import NNetCommand;

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
		if (!m_pStack->UndoCommand())
			m_pSound->Warning();
	}

private:

	inline static const wstring NAME { L"Undo" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			UndoCommand::Push();
		}
	} m_wrapper;

};

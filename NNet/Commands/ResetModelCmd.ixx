// ResetModelCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module ResetModelCmd;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class ResetModelCmd : public NNetCommand
{
public:

	ResetModelCmd()
	{}

	void Do() final
	{
	};

	void Undo() final
	{
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pNMWI->ResetModel();
		m_pStack->Clear();
	}

private:

	inline static const wstring NAME { L"ResetModel" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			ResetModelCmd::Push();
		}
	};
};

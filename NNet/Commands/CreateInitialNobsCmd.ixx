// CreateInitialNobsCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module CreateInitialNobsCmd;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class CreateInitialNobsCmd : public NNetCommand
{
public:

	CreateInitialNobsCmd()
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
		m_pNMWI->CreateInitialNobs();
	}

private:

	inline static const wstring NAME { L"CreateInitialNobs" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			CreateInitialNobsCmd::Push();
		}
	};
};

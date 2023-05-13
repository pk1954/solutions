// CreateInitialNobsCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:CreateInitialNobsCmd;

import Types;
import IoUtil;
import :NNetCommand;
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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pNMWI->CreateInitialNobs();
	}

private:

	inline static const wstring NAME { L"CreateInitialNobs" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			CreateInitialNobsCmd::Push();
		}
	} m_wrapper;
};

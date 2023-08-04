// CreateInitialNobsCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:CreateInitialNobsCmd;

import :NNetCommand;

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

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			CreateInitialNobsCmd::Push();
		}
	} m_wrapper { NAME };
};

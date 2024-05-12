// AddModuleCommand.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:AddModuleCommand;

import NNetModelIO;
import :NNetCommand;
import :AddNobsCommand;

export class AddModuleCommand : public NNetCommand
{
public:

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << SPACE << Quoted(NNetModelIO::GetModelFileName()) << SPACE << endl;
		PushCommand(make_unique<AddNobsCommand>(move(NNetModelIO::GetImportedModel()->MoveUPNobs())));
	}

private:

	inline static const wstring NAME { L"AddModule" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			AddModuleCommand::Push();
		}
	} m_wrapper { NAME };
};

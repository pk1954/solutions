// AddModuleCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:AddModuleCommand;

import NNetModelIO;
import :NNetCommand;
import :AddNobsCommand;

export class AddModuleCommand : public NNetCommand
{
public:

	AddModuleCommand()
	{}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << SPACE << DOUBLE_QUOTE << NNetModelIO::GetModelFileName() << DOUBLE_QUOTE << SPACE << endl;
		unique_ptr<Model> upImportedModel { NNetModelIO::GetImportedModel() };
		PushCommand(make_unique<AddNobsCommand>(upImportedModel->MoveUPNobs()));
	}

private:

	inline static const wstring NAME { L"AddModule" };

	inline static struct Wrapper : public WrapBase
	{
		using WrapBase::WrapBase;
		void operator() (Script& script) const final
		{
			AddModuleCommand::Push();
		}
	} m_wrapper { NAME };
};

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

	static void Register(NNetModelIO* const pModelIO)
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
		m_pModelIO = pModelIO;
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
		unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
		PushCommand(make_unique<AddNobsCommand>(upImportedModel->MoveUPNobs()));
	}

private:

	inline static const wstring NAME { L"AddModule" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			AddModuleCommand::Push();
		}
	} m_wrapper;

	inline static NNetModelIO* m_pModelIO;
};

// AddModuleCommand.ixx
//
// NNetCommands

export module NNetCommands:AddModuleCommand;

import std;
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
		PushCommand(make_unique<AddNobsCommand>(move(NNetModelIO::GetImportedModel()->m_upNobs)));
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

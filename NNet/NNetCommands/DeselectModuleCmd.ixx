// DeselectModuleCmd.ixx
//
// NNetCommands

export module NNetCommands:DeselectModuleCmd;

import std;
import :SelectionCommand;
import NNetModel;

export class DeselectModuleCmd : public SelectionCommand
{
public:
	DeselectModuleCmd()
	{ }

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().DeselectAllNobs();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		PushCommand(make_unique<DeselectModuleCmd>());
	}

private:

	inline static const wstring NAME { L"DeselectModule" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			DeselectModuleCmd::Push();
		}
	} m_wrapper { NAME };
};

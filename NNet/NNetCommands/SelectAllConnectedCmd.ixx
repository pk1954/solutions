// SelectAllConnectedCmd.ixx
//
// NNetCommands

export module NNetCommands:SelectAllConnectedCmd;

import std;
import IoUtil;
import IoConstants;
import :SelectionCommand;

export class SelectAllConnectedCmd : public SelectionCommand
{
public:
	SelectAllConnectedCmd(NobId const idNob)
	  : m_nobStart(*m_pNMWI->GetNobPtr<Nob *>(idNob))
	{}

	void Do() final
	{
		SelectionCommand::Do();    // not needed?
		m_nobStart.SelectAllConnected(true);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const idNob)
	{
		if (IsTraceOn())
			TraceStream() << NAME << SPACE << idNob << endl;
		PushCommand(make_unique<SelectAllConnectedCmd>(idNob));
	}

private:

	inline static const wstring NAME { L"SelectAllConnected" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SelectAllConnectedCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper { NAME };

	Nob& m_nobStart;
	bool m_bOn;
};

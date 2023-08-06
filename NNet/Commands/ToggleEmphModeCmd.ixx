// ToggleEmphModeCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:ToggleEmphModeCmd;

import :NNetCommand;
import NNetModel;
import IoUtil;

export class ToggleEmphModeCmd : public NNetCommand
{
public:
	explicit ToggleEmphModeCmd(NobId const id)
		: m_pPipe(m_pNMWI->GetNobPtr<Pipe*>(id))
	{
		assert(m_pPipe);
	}

	void Do() final
	{ 
		m_pPipe->Emphasize(!m_pPipe->IsEmphasized());
	}

	void Undo() final
	{ 
		Do();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const idNob)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idNob << endl;
		PushCommand(make_unique<ToggleEmphModeCmd>(idNob));
	}

private:

	inline static const wstring NAME { L"ToggleEmphMode" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			ToggleEmphModeCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper { NAME };

	Pipe * m_pPipe { nullptr };
};

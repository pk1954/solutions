// ToggleEmphModeCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module ToggleEmphModeCmd;

import NNetCommand;
import NNetModel;

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
			TraceStream() << NAME << L' ' << idNob.GetValue() << endl;
		m_pStack->PushCommand(make_unique<ToggleEmphModeCmd>(idNob));
	}

private:

	inline static const wstring NAME { L"ToggleEmphMode" };

	inline static struct Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			ToggleEmphModeCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper;

	Pipe * m_pPipe { nullptr };
};

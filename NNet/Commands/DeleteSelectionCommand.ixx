// DeleteSelectionCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module DeleteSelectionCommand;

import Types;
import CommandFunctions;
import Commands;
import NNetCommandStack;
import NNetCommand;
import NNetModel;

using std::unique_ptr;

export class DeleteSelectionCommand : public NNetCommand
{
public:
	DeleteSelectionCommand() = default;

	void Do() final 
	{ 
		m_cmdStack.Initialize(nullptr);
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>([this](Nob & nob) { doDelete(nob); });
	}

	void Undo() final 
	{ 
		m_cmdStack.UndoAll(); 
		m_cmdStack.Clear();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		m_pStack->PushCommand(make_unique<DeleteSelectionCommand>());
	}

private:

	inline static const wstring NAME { L"DeleteSelection" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			DeleteSelectionCommand::Push();
		}
	};

	void doDelete(Nob & nob)
	{ 
		if (unique_ptr<NNetCommand> upCmd { MakeDeleteCommand(*m_pNMWI, nob.GetId()) })
		{
			upCmd->Do();
			m_pNMWI->CheckModel();
			m_cmdStack.Push(move(upCmd));
		}
	}

	NNetCommandStack m_cmdStack {};
};
// DeleteSelectionCommand.ixx
//
// NNetCommands

module;

#include <memory>
#include <iostream>

export module NNetCommands:DeleteSelectionCommand;

import :DeleteNobCommand;
import Commands;
import :NNetCommand;

using std::unique_ptr;

export class DeleteSelectionCommand : public NNetCommand
{
public:
	DeleteSelectionCommand() = default;

	void Do() final 
	{ 
		m_cmdStack.Initialize(nullptr);
		m_pNMWI->GetUPNobs().Apply2AllSelected<Nob>
		(
			[this](Nob & nob) 
			{ 
				if (unique_ptr<NNetCommand> upCmd { DeleteNobCommand::MakeCommand(nob.GetId()) })
				{
					upCmd->Do();
					m_pNMWI->CheckModel();
					m_cmdStack.Push(move(upCmd));
				}
			}
		);
	}

	void Undo() final 
	{ 
		m_cmdStack.UndoAll(); 
		m_cmdStack.Clear();
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push()
	{
		if (IsTraceOn())
			TraceStream() << NAME << endl;
		PushCommand(make_unique<DeleteSelectionCommand>());
	}

private:

	inline static const wstring NAME { L"DeleteSelection" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			DeleteSelectionCommand::Push();
		}
	} m_wrapper { NAME };

	CommandStack m_cmdStack {};
};
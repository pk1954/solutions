// DeleteNobCommand.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:DeleteNobCommand;

import :NNetCommand;

export class DeleteNobCommand : public NNetCommand
{
public:
	DeleteNobCommand(NobId const nobId) {}

	~DeleteNobCommand() final = default;

	void Do  () final {}
	void Undo() final {}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static unique_ptr<NNetCommand> MakeCommand(NobId const);

	static void Push(NobId const id)
	{
		if (unique_ptr<NNetCommand> upCmd { MakeCommand(id) })
		{
			if (IsTraceOn())
				TraceStream() << NAME << id << endl;
			PlaySound(L"DISAPPEAR_SOUND");
			PushCommand(move(upCmd));
		}
	}

private:

	inline static const wstring NAME { L"DeleteNob" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			DeleteNobCommand::Push(ScrReadNobId(script));
		}
	} m_wrapper { NAME };
};

// DeleteNobCommand.ixx
//
// Commands

module;

#include <iostream>

export module DeleteNobCommand;

import NNetCommand;

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
				TraceStream() << NAME << id.GetValue() << endl;
			m_pSound->Play(L"DISAPPEAR_SOUND");
			m_pStack->PushCommand(move(upCmd));
		}
	}

private:

	inline static const wstring NAME { L"DeleteNob" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			DeleteNobCommand::Push(ScrReadNobId(script));
		}
	} m_wrapper;
};

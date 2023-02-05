// ToggleStopOnTriggerCmd.ixx
//
// Commands

module;

#include <iostream>

export module ToggleStopOnTriggerCmd;

import NNetCommand;
import NNetModel;

export class ToggleStopOnTriggerCmd : public NNetCommand
{
public:
	explicit ToggleStopOnTriggerCmd(NobId const id)
		:	m_id(id)
	{ }

	void Do() final
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
		m_pSound->Play(L"SNAP_IN_SOUND");
	}

	void Undo() final
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId const id)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << endl;
		m_pStack->PushCommand(make_unique<ToggleStopOnTriggerCmd>(id));
	}

private:

	inline static const wstring NAME { L"ToggleStopOnTrigger" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId const id { ScrReadNobId(script) };
			ToggleStopOnTriggerCmd::Push(id);
		}
	};

	NobId const m_id;
};


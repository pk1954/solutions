// ToggleStopOnTriggerCmd.ixx
//
// NNetCommands

export module NNetCommands:ToggleStopOnTriggerCmd;

import std;
import :NNetCommand;
import :NNetSound;
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
		PlaySound(SNAP_IN_SOUND);
	}

	void Undo() final
	{
		m_pNMWI->ToggleStopOnTrigger(m_id);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const id)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << endl;
		PushCommand(make_unique<ToggleStopOnTriggerCmd>(id));
	}

private:

	inline static const wstring NAME { L"ToggleStopOnTrigger" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId const id { ScrReadNobId(script) };
			ToggleStopOnTriggerCmd::Push(id);
		}
	} m_wrapper { NAME };

	NobId const m_id;
};

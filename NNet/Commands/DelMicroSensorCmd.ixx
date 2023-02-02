// DelMicroSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module DelMicroSensorCmd;

import NNetCommand;

export class DelMicroSensorCmd : public NNetCommand
{
public:

	DelMicroSensorCmd
	(
		NobId const nobId
	)
		: m_nobId(nobId)
	{}

	void Do() final
	{
	};

	void Undo() final
	{
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push
	(
		NobId const nobId
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << endl;
		m_pStack->PushCommand(make_unique<DelMicroSensorCmd>(nobId));
	}

private:

	inline static const wstring NAME { L"DelMicroSensor" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			DelMicroSensorCmd::Push(ScrReadNobId(script));
		}
	};

	NobId const m_nobId;
};

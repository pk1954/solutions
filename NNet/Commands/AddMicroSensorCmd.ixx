// AddMicroSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module AddMicroSensorCmd;

import NNetCommand;

export class AddMicroSensorCmd : public NNetCommand
{
public:

	AddMicroSensorCmd
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
		m_pStack->PushCommand(make_unique<AddMicroSensorCmd>(nobId));
	}

private:

	inline static const wstring NAME { L"AddMicroSensor" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			AddMicroSensorCmd::Push(ScrReadNobId(script));
		}
	};

	NobId const m_nobId;
};

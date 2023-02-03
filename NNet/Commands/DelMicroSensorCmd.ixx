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
	{
		m_microSensorId = m_pNMWI->GetMicroSensorList().FindMicroSensor(nobId);
	}

	void Do() final
	{
		m_upMicroSensor = move(m_pNMWI->GetMicroSensorList().RemoveMicroSensor(m_microSensorId));
		m_signalId = m_pNMWI->FindSignalId
		(
			[this](Signal const& s)
			{ return s.GetSignalSource() == m_upMicroSensor.get(); }
		);
		m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
		m_pSound->Play(L"DISAPPEAR_SOUND");
	};

	void Undo() final
	{
		m_pNMWI->GetMicroSensorList().InsertMicroSensor(move(m_upMicroSensor), m_microSensorId);
		m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
		m_pSound->Play(L"SNAP_IN_SOUND");
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
	MicroSensorId           m_microSensorId {};
	SignalId                m_signalId {};
	unique_ptr<MicroSensor> m_upMicroSensor {};
	unique_ptr<Signal>      m_upSignal {};
};

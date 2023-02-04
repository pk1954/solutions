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

	DelMicroSensorCmd(NobId const nobId)
	{
		m_sensorId = m_pNMWI->GetSensorList().FindSensor(nobId);
	}

	void Do() final
	{
		m_upSensor = move(m_pNMWI->GetSensorList().RemoveSensor(m_sensorId));
		m_signalId = m_pNMWI->FindSignalId
		(
			[this](Signal const& s)
			{ return s.GetSignalSource() == m_upSensor.get(); }
		);
		m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
		m_pSound->Play(L"DISAPPEAR_SOUND");
	};

	void Undo() final
	{
		m_pNMWI->GetSensorList().InsertSensor(move(m_upSensor), m_sensorId);
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

	SensorId           m_sensorId {};
	SignalId           m_signalId {};
	unique_ptr<Sensor> m_upSensor {};
	unique_ptr<Signal> m_upSignal {};
};

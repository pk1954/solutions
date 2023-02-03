// SizeSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module SizeSensorCmd;

import NNetCommand;
import NNetModel;

export class SizeSensorCmd : public NNetCommand
{
public:
	SizeSensorCmd
	(
		SensorId const & id,
		float    const   fFactor
	)
	  : m_pSensor(m_pNMWI->GetSensorList().GetSensor(id)),
		m_sensorId(id),
		m_fFactor(fFactor)
	{}

	void Do() final 
	{ 
		if (m_pSensor)
			m_pSensor->SizeSensor(m_pNMWI->GetUPNobsC(), 1.0f / m_fFactor); 
	}

	void Undo() final 
	{ 
		if (m_pSensor)
			m_pSensor->SizeSensor(m_pNMWI->GetUPNobsC(), m_fFactor); 
	}

	bool CombineCommands(Command const & src) final
	{ 
		SizeSensorCmd const & cmdSrc { static_cast<SizeSensorCmd const &>(src) };
		if (m_sensorId != cmdSrc.m_sensorId)
			return false;
		m_fFactor *= cmdSrc.m_fFactor;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(SensorId id, float const fFactor)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << L" " << fFactor << endl;
		m_pStack->PushCommand(make_unique<SizeSensorCmd>(id, fFactor));
	}

private:

	inline static const wstring NAME { L"SizeSensor" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SensorId const id(script.ScrReadUint());
			float    const fFactor { Cast2Float(script.ScrReadFloat()) };
			SizeSensorCmd::Push(id, fFactor);
		}
	};

	Sensor * m_pSensor;
	SensorId m_sensorId;
	float    m_fFactor;
};

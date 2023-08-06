// SizeSensorCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:SizeSensorCmd;

import :NNetCommand;
import NNetModel;
import SaveCast;
import IoConstants;
import IoUtil;

export class SizeSensorCmd : public NNetCommand
{
public:
	SizeSensorCmd
	(
		SensorId const & id,
		float    const   fFactor
	)
	  : m_sensorId(id),
		m_fFactor(fFactor)
	{
		Sensor* pSensor { m_pNMWI->GetSensorList().GetSensor(id) };
		assert(pSensor->IsSensor());
		m_pSensor = static_cast<Sensor*>(pSensor);
	}

	void Do() final 
	{ 
		m_pSensor->SizeSensor(m_pNMWI->GetUPNobsC(), 1.0f / m_fFactor); 
	}

	void Undo() final 
	{ 
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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(SensorId id, float const fFactor)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << SPACE << fFactor << endl;
		PushCommand(make_unique<SizeSensorCmd>(id, fFactor));
	}

private:

	inline static const wstring NAME { L"SizeSensor" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SensorId const id      { script.ScrReadInt() };
			float    const fFactor { Cast2Float(script.ScrReadFloat()) };
			SizeSensorCmd::Push(id, fFactor);
		}
	} m_wrapper { NAME };

	Sensor * m_pSensor;
	SensorId m_sensorId;
	float    m_fFactor;
};

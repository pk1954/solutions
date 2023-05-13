// MoveSensorCmd.ixx
//
// Commands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:MoveSensorCmd;

import IoUtil;
import :NNetCommand;

export class MoveSensorCmd : public NNetCommand
{
public:
	MoveSensorCmd
	(
		SensorId      const & id,
		MicroMeterPnt const & delta
	)
	  : m_delta(delta)
	{
		Sensor * pSensor { m_pNMWI->GetSensorList().GetSensor(id) };
		assert(pSensor);
		assert(pSensor->IsSensor());
		m_pSensor = static_cast<Sensor *>(pSensor);
	}

	void Do() final 
	{ 
		m_pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), m_delta);
	}

	void Undo() final 
	{ 
		m_pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), -m_delta);
	}

	bool CombineCommands(Command const & src) final
	{ 
		MoveSensorCmd const & cmdSrc { static_cast<MoveSensorCmd const &>(src) };
		if (m_pSensor != cmdSrc.m_pSensor)
			return false;
		m_delta += cmdSrc.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(SensorId id, MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id.GetValue() << L' ' << delta << endl;
		PushCommand(make_unique<MoveSensorCmd>(id, delta));
	}

private:

	inline static const wstring NAME { L"MoveSensor" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			SensorId      const id      { script.ScrReadInt() };
			MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
			MoveSensorCmd::Push(id, umDelta);
		}
	} m_wrapper;

	Sensor * m_pSensor;
	MicroMeterPnt m_delta;
};

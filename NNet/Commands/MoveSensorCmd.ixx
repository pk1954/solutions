// MoveSensorCmd.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module MoveSensorCmd;

import Types;
import Symtab;
import Script;
import Commands;
import NNetWrapperHelpers;
import NNetCommand;
import NNetModel;

using std::endl;
using std::wstring;
using std::make_unique;
using std::unique_ptr;

export class MoveSensorCmd : public NNetCommand
{
public:
	MoveSensorCmd
	(
		SensorId      const & id,
		MicroMeterPnt const & delta
	)
	  : m_sensorId(id),
		m_delta(delta)
	{}

	void Do() final 
	{ 
		Sensor * pSensor { m_pNMWI->GetSensorList().GetSensor(m_sensorId) };
		if (pSensor)
			pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), m_delta);
	}

	void Undo() final 
	{ 
		Sensor * pSensor { m_pNMWI->GetSensorList().GetSensor(m_sensorId) };
		if (pSensor)
			pSensor->MoveSensor(m_pNMWI->GetUPNobsC(), -m_delta);
	}

	bool CombineCommands(Command const & src) final
	{ 
		MoveSensorCmd const & cmdSrc { static_cast<MoveSensorCmd const &>(src) };
		if (m_sensorId != cmdSrc.m_sensorId)
			return false;
		m_delta += cmdSrc.m_delta;
		return true; 
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(SensorId id, MicroMeterPnt const& delta)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << delta << endl;
		m_pStack->PushCommand(make_unique<MoveSensorCmd>(id, delta));
	}

private:

	inline static const wstring NAME { L"MoveSensor" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SensorId      const id(script.ScrReadUint());
			MicroMeterPnt const umDelta { ScrReadMicroMeterPnt(script) };
			MoveSensorCmd::Push(id, umDelta);
		}
	};

	SensorId      m_sensorId;
	MicroMeterPnt m_delta;
};

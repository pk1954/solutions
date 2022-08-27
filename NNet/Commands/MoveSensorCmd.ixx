// MoveSensorCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"

export module MoveSensorCmd;

import MoreTypes;
import UPSensorList;
import NNetCommand;
import Sensor;

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

private:
	SensorId      m_sensorId;
	MicroMeterPnt m_delta;
};

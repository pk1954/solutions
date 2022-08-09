// SizeSensorCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Signal.h"
#include "MonitorData.h"

export module SizeSensorCmd;

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

private:
	Sensor * m_pSensor;
	SensorId m_sensorId;
	float    m_fFactor;
};

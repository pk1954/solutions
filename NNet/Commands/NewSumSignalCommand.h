// NewSumSignalCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "SumSignal.h"

class NewSumSignalCommand : public Command
{
public:
	NewSumSignalCommand( MicroMeterPoint const & pos )
	{ 
//		m_upSensor = make_unique<Sensor>( MicroMeterCircle(pos, 0.0_MicroMeter) );
	}

	~NewSumSignalCommand( ) {}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
//		m_sensorId = nmwi.AddSensor( m_upSensor );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
//		m_upSensor = nmwi.RemoveSensor( m_sensorId );
	}

private:
//	SensorId           m_sensorId { -1 };
//	unique_ptr<Sensor> m_upSensor { nullptr };
};


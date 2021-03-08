// ConnectionNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ConnectionNeuron.h"

MicroMeterPoint const ConnectionNeuron::determineVector() const
{
	MicroMeterPoint umVector { 0._MicroMeter, 1._MicroMeter };

	if ( m_connections.HasConnection() )
	{
		Radian rad { 0._Radian };
		m_connections.Apply2AllConnectedPipes( [&](Pipe & pipe) { rad += Vector2Radian(pipe.GetVector()); } );
		rad /= Cast2Float(m_connections.GetNrOfConnections());
		umVector = Radian2Vector(rad);
	}

	return umVector;
}

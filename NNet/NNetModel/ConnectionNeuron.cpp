// ConnectionNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ConnectionNeuron.h"

MicroMeterPoint const ConnectionNeuron::determineVector() const
{
	MicroMeterPoint umVector { MicroMeterPoint::ZERO_VAL() };

	m_connections.Apply2AllConnectedPipes
	( 
		[&](Pipe & pipe) { umVector += Normalize(pipe.GetVector());	} 
	);

	if ( umVector.IsZero() )
		umVector = MicroMeterPoint( 0.0_MicroMeter, 1.0_MicroMeter );

	return umVector;
}

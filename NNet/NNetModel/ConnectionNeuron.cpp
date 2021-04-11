// ConnNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ConnectionNeuron.h"

MicroMeterPoint const ConnNeuron::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPoint const ConnNeuron::determineVector() const
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

void ConnNeuron::RotateShape( MicroMeterPoint const & umPntPivot, Radian const radDelta )
{
	BaseKnot::RotateShape(umPntPivot, radDelta);
	m_radDirection += radDelta;
}


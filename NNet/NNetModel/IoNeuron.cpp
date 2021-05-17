// IoNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "IoNeuron.h"

MicroMeterPoint const IoNeuron::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPoint const IoNeuron::determineVector() const
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

void IoNeuron::RotateNob( MicroMeterPoint const & umPntPivot, Radian const radDelta )
{
	BaseKnot::RotateNob(umPntPivot, radDelta);
	m_radDirection += radDelta;
}


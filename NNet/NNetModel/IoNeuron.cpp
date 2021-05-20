// IoNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "IoNeuron.h"

MicroMeterPnt const IoNeuron::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPnt const IoNeuron::determineVector() const
{
	MicroMeterPnt umVector { MicroMeterPnt::ZERO_VAL() };

	m_connections.Apply2AllConnectedPipes
	( 
		[&](Pipe & pipe) { umVector += Normalize(pipe.GetVector());	} 
	);

	if ( umVector.IsZero() )
		umVector = MicroMeterPnt( 0.0_MicroMeter, 1.0_MicroMeter );

	return umVector;
}

void IoNeuron::RotateNob( MicroMeterPnt const & umPntPivot, Radian const radDelta )
{
	BaseKnot::RotateNob(umPntPivot, radDelta);
	m_radDirection += radDelta;
}


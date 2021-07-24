// IoNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Pipe.h"
#include "IoNeuron.h"

MicroMeterPnt const IoNeuron::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPnt const IoNeuron::determineVector() const
{
	MicroMeterPnt umVector { MicroMeterPnt::ZERO_VAL() };

	Apply2AllConnectedPipes
	( 
		[&](Pipe & pipe) { umVector += Normalize(pipe.GetVector());	} 
	);

	if ( umVector.IsZero() )
		umVector = MicroMeterPnt( 0.0_MicroMeter, 1.0_MicroMeter );

	return umVector;
}

MicroMeterPnt const IoNeuron::GetDirVector() const
{
	MicroMeterPnt umVector 
	{ 
		m_radDirection.IsNull() 
		? determineVector() 
		: Radian2Vector(m_radDirection) 
	};
	return umVector.ScaledTo(GetExtension());
}

Radian const IoNeuron::GetDir() const 
{ 
	return m_radDirection.IsNull() 
		? Vector2Radian(determineVector()) 
		: m_radDirection;
};

void IoNeuron::RotateNob( MicroMeterPnt const & umPntPivot, Radian const radDelta )
{
	BaseKnot::RotateNob(umPntPivot, radDelta);
	m_radDirection += radDelta;
}

MicroMeterPosDir const IoNeuron::GetRawPosDir() const
{
	return MicroMeterPosDir(GetPos(), m_radDirection);
}

MicroMeterPosDir const IoNeuron::GetPosDir() const
{
	return MicroMeterPosDir(GetPos(), Vector2Radian(GetDirVector()));
}

void IoNeuron::UnlockDirection() 
{
	m_radDirection.Set2Null();
}


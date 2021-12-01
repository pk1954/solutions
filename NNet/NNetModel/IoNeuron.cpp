// IoNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Pipe.h"
#include "IoNeuron.h"

MicroMeterPnt IoNeuron::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPnt IoNeuron::determineVector() const
{
	MicroMeterPnt umVector { MicroMeterPnt::ZERO_VAL() };

	Apply2AllConnectedPipes
	(
		[&umVector](Pipe const & pipe) { umVector += Normalize(pipe.GetVector()); } 
	);

	if (umVector.IsZero())
		umVector = MicroMeterPnt(0.0_MicroMeter, 1.0_MicroMeter);

	return umVector;
}

MicroMeterPnt IoNeuron::GetDirVector() const
{
	MicroMeterPnt umVector 
	{ 
		m_radDirection.IsNull() 
		? determineVector() 
		: Radian2Vector(m_radDirection) 
	};
	return umVector.ScaledTo(GetExtension());
}

Radian IoNeuron::GetDir() const 
{ 
	return (IsDirLocked())
		? m_radDirection
		: Vector2Radian(determineVector());
};

void IoNeuron::RotateNob(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
	BaseKnot::RotateNob(umPntPivot, radDelta);
	m_radDirection += radDelta;
}

MicroMeterPosDir IoNeuron::GetRawPosDir() const
{
	return MicroMeterPosDir(GetPos(), m_radDirection);
}

MicroMeterPosDir IoNeuron::GetPosDir() const
{
	return MicroMeterPosDir(GetPos(), Vector2Radian(GetDirVector()));
}

void IoNeuron::UnlockDirection() 
{
	m_radDirection.Set2Null();
}

void IoNeuron::LockDirection() 
{ 
	if (!IsDirLocked()) 
		m_radDirection = Vector2Radian(determineVector());
}

void IoNeuron::SetDir(Radian const r) 
{ 
	m_radDirection = r; 
}

bool IoNeuron::IsDirLocked() const
{
	return m_radDirection.IsNotNull();
}

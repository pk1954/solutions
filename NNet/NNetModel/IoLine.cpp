// IoLine.cpp 
//
// NNetModel

module;

#include "MicroMeterPosDir.h"
#include "Pipe.h"

module IoLine;

import MoreTypes;

MicroMeterPnt IoLine::GetScaledDirVector() const
{
	return GetDirVector().ScaledTo(GetExtension());
}

MicroMeterPnt IoLine::determineVector() const
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

MicroMeterPnt IoLine::GetDirVector() const
{
	MicroMeterPnt umVector 
	{ 
		m_radDirection.IsNull() 
		? determineVector() 
		: Radian2Vector(m_radDirection) 
	};
	return umVector.ScaledTo(GetExtension());
}

Radian IoLine::GetDir() const 
{ 
	return (IsDirLocked())
		? m_radDirection
		: Vector2Radian(determineVector());
};

MicroMeterPosDir IoLine::GetRawPosDir() const
{
	return MicroMeterPosDir(GetPos(), m_radDirection);
}

MicroMeterPosDir IoLine::GetPosDir() const
{
	return MicroMeterPosDir(GetPos(), Vector2Radian(GetDirVector()));
}

void IoLine::UnlockDirection() 
{
	SetDir(Radian::NULL_VAL());
}

void IoLine::LockDirection() 
{ 
	if (!IsDirLocked()) 
		SetDir(Vector2Radian(determineVector()));
}

void IoLine::SetDir(Radian const r) 
{ 
	m_radDirection = r; 
}

bool IoLine::IsDirLocked() const
{
	return m_radDirection.IsNotNull();
}

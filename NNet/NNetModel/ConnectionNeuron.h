// ConnectionNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "Neuron.h"

using std::wcout;
using std::endl;

class ConnNeuron : public Neuron
{
public:
	static bool const TypeFits(ShapeType const type) { return type.IsConnNeuronType(); }

	ConnNeuron( MicroMeterPoint const & upCenter, ShapeType const type )
		: Neuron( upCenter, type )
	{}

	void Check() const
	{
		Neuron::CheckShape();
	}

	virtual void RotateShape(MicroMeterPoint const &, Radian const);

	void SetDirVector(MicroMeterPoint const umVector)
	{
		m_radDirection = Vector2Radian(umVector);
	}

	MicroMeterPoint const GetDirVector() const
	{
		MicroMeterPoint umVector 
		{ 
			m_radDirection.IsNull() 
			? determineVector() 
			: Radian2Vector(m_radDirection) 
		};
		return umVector.ScaledTo(GetExtension());
	}

	MicroMeterPosDir const GetRawPosDir() const
	{
		return MicroMeterPosDir( GetPos(), m_radDirection );
	}

	MicroMeterPosDir const GetPosDir() const
	{
		return MicroMeterPosDir( GetPos(), Vector2Radian(GetDirVector()) );
	}

	void SetPosDir( MicroMeterPosDir const posDir )
	{
		m_radDirection = posDir.GetDir();
		SetPos( posDir.GetPos() );
	}

	void UnlockDirection() 
	{
		m_radDirection.Set2Null();
	}

protected:
	MicroMeterPoint const GetScaledDirVector() const;

private:
	MicroMeterPoint const determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};

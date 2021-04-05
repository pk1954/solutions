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

	MicroMeterPoint const GetDirVector() const
	{
		MicroMeterPoint umVector { m_direction.IsNull() ? determineVector() : Radian2Vector(m_direction) };
		return Normalize(umVector) * GetExtension().GetValue();
	}

	MicroMeterPosDir const GetPosDir() const
	{
		return MicroMeterPosDir( GetPosition(), Vector2Radian(GetDirVector()) );
	}

	void SetPosDir( MicroMeterPosDir const posDir )
	{
		m_direction = posDir.GetDir();
		SetPosition ( posDir.GetPos() );
	}

	void UnlockDirection() 
	{
		m_direction.Set2Null();
	}

protected:
	MicroMeterPoint const GetScaledDirVector() const;

private:
	MicroMeterPoint const determineVector() const;

	Radian m_direction { Radian::NULL_VAL() };
};

using CNPtr = ConnNeuron *;

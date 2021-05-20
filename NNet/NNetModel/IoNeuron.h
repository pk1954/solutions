// IoNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "Neuron.h"

using std::wcout;
using std::endl;

class IoNeuron : public Neuron
{
public:
	static bool const TypeFits(NobType const type) { return type.IsIoNeuronType(); }

	IoNeuron( MicroMeterPnt const & upCenter, NobType const type )
		: Neuron( upCenter, type )
	{}

	void Check() const
	{
		Neuron::Check();
	}

	virtual void RotateNob(MicroMeterPnt const &, Radian const);

	void SetDir(Radian const radian)
	{
		m_radDirection = radian;
	}

	void SetDirVector(MicroMeterPnt const umVector)
	{
		SetDir(Vector2Radian(umVector));
	}

	MicroMeterPnt const GetDirVector() const
	{
		MicroMeterPnt umVector 
		{ 
			m_radDirection.IsNull() 
			? determineVector() 
			: Radian2Vector(m_radDirection) 
		};
		return umVector.ScaledTo(GetExtension());
	}

	virtual Radian const GetDir() const 
	{ 
		return m_radDirection.IsNull() 
			? Vector2Radian(determineVector()) 
			: m_radDirection;
	};

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
	MicroMeterPnt const GetScaledDirVector() const;

private:
	MicroMeterPnt const determineVector() const;

	Radian m_radDirection { Radian::NULL_VAL() };
};

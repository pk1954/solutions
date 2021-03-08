// ConnectionNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "Neuron.h"

class ConnectionNeuron : public Neuron
{
public:
	ConnectionNeuron( MicroMeterPoint const upCenter, ShapeType const type )
		: Neuron( upCenter, type )
	{ }

	MicroMeterPoint const GetDirVector() const
	{
		if ( m_umVector.IsNull() )
			SetDirection( determineVector() );
		return m_umVector;
	}

	MicroMeterPosDir const GetPosDir( ) const
	{
		return MicroMeterPosDir( GetPosition(), Vector2Radian(GetDirVector()) );
	}

	void SetPosDir( MicroMeterPosDir const posDir )
	{
		SetDirection( posDir.GetDir() );
		SetPosition ( posDir.GetPos() );
	}

	void SetDirection( MicroMeterPoint const umPnt ) const 
	{
		m_umVector = Normalize(umPnt) * GetExtension().GetValue();
	}

	void SetDirection( Radian const rad )
	{
		SetDirection(Radian2Vector( rad ));
	}

private:
	MicroMeterPoint const determineVector() const;

	mutable MicroMeterPoint m_umVector { MicroMeterPoint::NULL_VAL() };  // direction of ConnectionNeuron
};

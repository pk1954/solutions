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
	ConnectionNeuron( MicroMeterPoint const & upCenter, ShapeType const type )
		: Neuron( upCenter, type )
	{}

	MicroMeterPoint const GetDirVector() const
	{
		return m_umVector.IsNull() ? determineVector() : m_umVector;
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

	void SetDirection( MicroMeterPoint const umPnt ) 
	{
		m_umVector = Normalize(umPnt) * GetExtension().GetValue();
	}

	void SetDirection( Radian const rad )
	{
		SetDirection(Radian2Vector( rad ));
	}

	void UnlockDirection() 
	{
		m_umVector = MicroMeterPoint::NULL_VAL();
	}

private:
	MicroMeterPoint const determineVector() const;

	MicroMeterPoint m_umVector { MicroMeterPoint::NULL_VAL() };  // direction of ConnectionNeuron
};

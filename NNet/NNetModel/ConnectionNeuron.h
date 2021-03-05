// ConnectionNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "Neuron.h"

class ConnectionNeuron : public Neuron
{
public:
	ConnectionNeuron( MicroMeterPoint const upCenter, ShapeType const type )
		: Neuron( upCenter, type )
	{ }

protected:
	MicroMeterPoint m_umVector { MicroMeterPoint::NULL_VAL() };  // direction of neuron if plug or socket

	MicroMeterPoint const DetermineVector(Connections::Type const conType) const
	{
		MicroMeterPoint umVector { MicroMeterPoint::ZERO_VAL() };

		if (m_umVector.IsNotNull())
			umVector = m_umVector;
		else if ( m_connections.HasConnection(conType) )
			m_connections.Apply2AllPipes( conType, [&](Pipe & pipe) { umVector += pipe.GetVector(); } );
		else
			umVector = { 0._MicroMeter, 1._MicroMeter };

		return Normalize(umVector) * GetExtension().GetValue();
	}

private:
};

// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Shape.h"
#include "BaseKnot.h"

using std::chrono::microseconds;

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const npCenter )
	  : BaseKnot( tShapeType::neuron, npCenter, 50.0_MicroMeter )
	{ 
	}

	virtual void Prepare( NNetModel & ) {};

	virtual void Step( ) { }

	virtual mV GetNextOutput( ) const
	{
		assert( m_mVinputBuffer <= PEAK_VOLTAGE );
		return m_mVinputBuffer;
	}

	virtual void Draw( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const {};

private:
};

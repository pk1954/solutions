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
	Neuron( MicroMeterPoint const, tShapeType const = tShapeType::neuron );

	virtual void Prepare( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;

protected:
	microseconds m_timeSinceLastPulse;

	mV waveFunction( microseconds ) const;

	void drawExterior( int const ) const;
	void drawInterior( int const ) const;

private:

};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );

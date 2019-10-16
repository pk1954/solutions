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

	virtual void Prepare( NNetModel & );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;
	virtual void DrawInterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const & ) const;

protected:
	microseconds m_timeSinceLastPulse;

	mV waveFunction( microseconds ) const;

	void drawExterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const &, int const ) const;
	void drawInterior( NNetModel const &, GraphicsInterface &, PixelCoordsFp const &, int const ) const;

private:

};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );

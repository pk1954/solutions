// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Shape.h"
#include "BaseKnot.h"

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const, tShapeType const = tShapeType::neuron );

	virtual void Prepare      ( NNetModel const & );
	virtual void Step         ( NNetModel const & );
	virtual mV   GetNextOutput( NNetModel const & ) const;

	virtual void DrawExterior( NNetModel const &, PixelCoordsFp & ) const;
	virtual void DrawInterior( NNetModel const &, PixelCoordsFp & ) const;

protected:
	MicroSecs m_timeSinceLastPulse;

	mV waveFunction( NNetModel const &, MicroSecs const ) const;

	void drawExterior( PixelCoordsFp &, int const ) const;
	void drawInterior( NNetModel const &, PixelCoordsFp &, int const ) const;

private:

};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );

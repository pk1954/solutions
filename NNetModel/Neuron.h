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
	Neuron( NNetModel *, MicroMeterPoint const, tShapeType const = tShapeType::neuron );
	virtual ~Neuron() {}

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::neuron;
	}

	virtual void Prepare( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( PixelCoordsFp & ) const;
	virtual void DrawInterior( PixelCoordsFp & ) const;

protected:
	MicroSecs m_timeSinceLastPulse;

	mV waveFunction( MicroSecs const ) const;

	void drawExterior( PixelCoordsFp &, int const ) const;
	void drawInterior( PixelCoordsFp &, int const ) const;

private:

	MicroMeterPoint getAxonHillockPos( PixelCoordsFp & ) const;
};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );

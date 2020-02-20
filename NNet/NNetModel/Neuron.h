// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "BaseKnot.h"

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const, tShapeType const = tShapeType::neuron );
	virtual ~Neuron() {}

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::neuron;
	}

	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( PixelCoordsFp &, tHighlightType const ) const;
	virtual void DrawInterior( PixelCoordsFp & );
	virtual void Recalc( );

protected:
	MicroSecs m_timeSinceLastPulse;

	mV waveFunction( MicroSecs const ) const;

	void drawExterior( PixelCoordsFp &, tHighlightType const ) const;
	void drawInterior( PixelCoordsFp & ) const;

private:
	float m_factorW;
	float m_factorU;

	MicroMeterPoint getAxonHillockPos( PixelCoordsFp & ) const;
};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );

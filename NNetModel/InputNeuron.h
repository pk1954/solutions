// InputNeuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Knot.h"

using std::chrono::microseconds;

class InputNeuron : public Knot
{
public:

	InputNeuron( MicroMeterPoint const );

	mV      Step( );
	void    Trigger( );
	PERCENT GetFillLevel( ) const;
	void    Draw( GraphicsInterface &,	PixelCoordsFp const & ) const;

	Hertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( Hertz const freq )
	{
		m_pulseFrequency = freq;
	}

private:

	microseconds m_timeSinceLastPulse;
	Hertz        m_pulseFrequency;

	mV waveFunction( microseconds );
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );

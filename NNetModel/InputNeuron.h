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

	virtual void Prepare( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	void    Trigger( );
	PERCENT GetFillLevel( ) const;
	void    Draw( GraphicsInterface &, PixelCoordsFp const & ) const;

	Hertz GetPulseFrequency( ) const
	{
		return m_pulseFrequency;
	}

	void SetPulseFrequency( Hertz const freq )
	{
		m_pulseFrequency = freq;
		m_pulseDuration = PulseDuration( m_pulseFrequency );
	}

private:

	microseconds m_timeSinceLastPulse;
	Hertz        m_pulseFrequency;
	microseconds m_pulseDuration;   // inverse of pulse frequency 

	mV waveFunction( microseconds ) const;
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );

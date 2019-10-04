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
		setSteps();
	}

private:

	microseconds m_timeSinceTrigger;
	bool         m_fTriggered;
	int          m_iCounter;
	int          m_iStepsBetweenTrigger;
	Hertz        m_pulseFrequency;

	void setSteps( )
	{
		m_iStepsBetweenTrigger = 
			CastToInt
			( 
			    microseconds::period::den / 
				( TIME_RESOLUTION.count() * m_pulseFrequency.GetValue() ) 
			);
	}

	double waveFunction( double x )
	{
		return 255.0 * x / ( x * x + 1.0 );
	}
};	

InputNeuron const * Cast2InputNeuron( Shape const * );
InputNeuron       * Cast2InputNeuron( Shape       * );

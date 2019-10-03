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

private:

	microseconds m_timeSinceTrigger;
	bool         m_fTriggered;
	int          m_iCounter;
	int          m_iStepsBetweenTrigger;
	MilliHertz   m_pulseFrequency;

	double waveFunction( double x )
	{
		return 255.0 * x / ( x * x + 1.0 );
	}
};
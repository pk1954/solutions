// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Knot.h"

using std::chrono::microseconds;

class Neuron : public Knot
{
public:
	Neuron( MicroMeterPoint const npCenter )
	  : Knot( npCenter ),
		m_fTriggered( false ),
		m_timeSinceTrigger( microseconds( 0 ) )
	{
	}

	void Trigger( )
	{
		m_fTriggered = true;
		m_timeSinceTrigger = microseconds( 0 );
	}

	virtual mV GetNextPotential( )
	{
		static microseconds const AP_DURATION = microseconds( 4000 );

		mV mVresult = BASE_POTENTIAL;
		if ( m_fTriggered )
		{
			mVresult += mV( waveFunction( static_cast<double>(m_timeSinceTrigger.count()) / 1000.0 ) );
			m_timeSinceTrigger += TIME_RESOLUTION;
			if ( m_timeSinceTrigger > AP_DURATION )  
				m_fTriggered = false;
		}
		return mVresult;
	}

private:
	microseconds m_timeSinceTrigger;
	bool         m_fTriggered;

	double waveFunction( double x )
	{
		return 256.0 * x / ( x * x + 1.0 );
	}
};
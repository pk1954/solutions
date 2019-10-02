// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Shape.h"
#include "Knot.h"

using std::chrono::microseconds;

class Neuron : public Knot
{
public:
	Neuron( MicroMeterPoint const npCenter )
	  : Knot( npCenter ),
		m_fTriggered( false ),
		m_timeSinceTrigger( microseconds( 0 ) ),
		m_iCounter( 0 ),
		m_pulseFrequency( MilliHertz( 50000 ) )
	{ 
		m_iStepsBetweenTrigger = 
			CastToInt
			( 
				( microseconds::period::den * 100 )
				/ ( TIME_RESOLUTION.count() * m_pulseFrequency.GetValue() ) 
			);
	}

	void Step( )
	{
		if ( m_iCounter == 0 )
		{
			Trigger();
			m_iCounter = m_iStepsBetweenTrigger;
		}
		else
		{
			--m_iCounter;
		}
	}

	void Trigger( )
	{
		m_fTriggered = true;
		m_timeSinceTrigger = microseconds( 0 );
	}

	PERCENT GetFillLevel( ) const
	{
		return PERCENT( (m_iCounter * 100 )/ m_iStepsBetweenTrigger );
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
	int          m_iCounter;
	int          m_iStepsBetweenTrigger;
	MilliHertz   m_pulseFrequency;

	double waveFunction( double x )
	{
		return 256.0 * x / ( x * x + 1.0 );
	}
};
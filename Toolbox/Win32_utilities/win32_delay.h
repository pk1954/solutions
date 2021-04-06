// win32_delay.h 
//
// win32_utilities

#pragma once

#include <synchapi.h>
#include <chrono>
#include "util.h"
#include "observable.h"

using std::chrono::milliseconds;

milliseconds const DEFAULT_DELAY( 50 );  
milliseconds const MAX_DELAY  ( 2048 );  

class Delay: public Observable
{
public:

	Delay()
	  : m_msGenerationDelay( DEFAULT_DELAY )
	{}

	milliseconds GetDelay()
	{
		return m_msGenerationDelay;
	};

	bool IsMaxSpeed() const
	{
		return m_msGenerationDelay == milliseconds::zero();
	};

	void SetDelay( milliseconds delay )
	{
		m_msGenerationDelay = delay;
		NotifyAll( false );
	};

	void SleepDelay() const
	{
		if ( m_msGenerationDelay > milliseconds::zero() )
			Sleep( Cast2UnsignedLong(m_msGenerationDelay.count()) );
	};

private:

	milliseconds m_msGenerationDelay; 
};

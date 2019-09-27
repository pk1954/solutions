// win32_delay.h 
//
// win32_utilities

#pragma once

#include <chrono>
#include "util.h"
#include "windows.h"
#include "ViewCollection.h"

using std::chrono::milliseconds;

milliseconds const DEFAULT_DELAY( 50 );  
milliseconds const MAX_DELAY  ( 2048 );  

class Delay
{
public:

	Delay()
	  : m_msGenerationDelay( DEFAULT_DELAY )
	{}

	milliseconds GetDelay( )
	{
		return m_msGenerationDelay;
	};

	BOOL IsMaxSpeed( ) const
	{
		return m_msGenerationDelay == milliseconds::zero();
	};

	void SetDelay( milliseconds delay )
	{
		m_msGenerationDelay = delay;
		m_observers.NotifyAll( false );
	};

	void SleepDelay( ) const
	{
		if ( m_msGenerationDelay > milliseconds::zero() )
			Sleep( CastToUnsignedLong(m_msGenerationDelay.count()) );
	};

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void Stop( )
	{
		m_observers.Clear();
	}

private:

	milliseconds   m_msGenerationDelay; 
	ViewCollection m_observers;
};

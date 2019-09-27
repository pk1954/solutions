// win32_actionTimer.h 
//
// win32_utilities

#pragma once

#include <chrono>
#include "util.h"
#include "NamedType.h"
#include "windows.h"
#include "win32_hiResTimer.h"
#include "ViewCollection.h"

using std::chrono::milliseconds;

using MilliHertz = NamedType< unsigned long, struct MilliHertz_Parameter >;

class ActionTimer
{
public:
	ActionTimer()
		: m_hrtimerSingleAction( ),
	      m_usSingleActionTime( 0 ),
		  m_hrtimerOverall( ),
		  m_dwCounter( 0 )
	{
		m_hrtimerOverall.Start( );
	}

	void TimerStart( )
	{
		m_hrtimerSingleAction.Start( );
	};

	void TimerStop( )
	{
		m_hrtimerSingleAction.Stop( );
		m_usSingleActionTime = m_hrtimerSingleAction.GetDuration( );
		++m_dwCounter;
		m_observers.NotifyAll( false );
	};

	microseconds GetSingleActionTime( )
	{
		return m_usSingleActionTime;
	}

	MilliHertz CalcActionFrequency( microseconds us, DWORD dwCount = 1 )
	{
		static unsigned long long MICROSECONDS_TO_MILLIHERTZ_FACTOR = 1000ull * 1000ull  * 1000ull;
		if ( us == microseconds::zero() )
			return  MilliHertz(0);

		assert( dwCount < ULLONG_MAX / MICROSECONDS_TO_MILLIHERTZ_FACTOR );    // avoid ull overflow
		unsigned long long ullFrequency = ( dwCount * MICROSECONDS_TO_MILLIHERTZ_FACTOR ) / us.count();
		return MilliHertz( CastToLong(ullFrequency) );
	}

	MilliHertz GetMeasuredPerformance( )
	{
		m_hrtimerOverall.Stop( );
		microseconds usOverallTime = m_hrtimerOverall.GetDuration( );
		MilliHertz  result         = CalcActionFrequency( usOverallTime, m_dwCounter );
		m_dwCounter = 0;
		m_hrtimerOverall.Start( );
		return result;
	}

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void Stop( )
	{
		m_observers.Clear();
	}

private:

	HiResTimer     m_hrtimerSingleAction;
	HiResTimer     m_hrtimerOverall;
	microseconds   m_usSingleActionTime; 
	DWORD          m_dwCounter;            // nr of executions
	ViewCollection m_observers;
};

// win32_hiResTimer.h : 
//
// win32_utilities

#pragma once

#include <chrono>
#include "NamedType.h"
#include "MOreTypes.h"

using std::chrono::microseconds;

using Ticks = NamedType< long long, struct Ticks_Parameter >;

class HiResTimer
{
public:
    HiResTimer( );
    ~HiResTimer( ) {};

	void  Start( );
	Ticks GetTicksTilStart( );
	void  Stop( );
	void  Restart( )
	{
		Stop();
		Start();
	}

	Ticks ReadHiResTimer( ) const;

	microseconds GetDuration( );
	microseconds TicksToMicroseconds( Ticks        const ) const;
	MicroSecs    TicksToMicroSecs   ( Ticks        const ) const;
	Ticks        MicroSecondsToTicks( microseconds const ) const;
	Ticks        MicroSecsToTicks   ( MicroSecs    const ) const;

	void BusyWait( microseconds const, Ticks & );

private:
	long long const MICROSECONDS_TO_SECONDS = microseconds::period::den;

    static Hertz m_frequency;

	bool  m_bStarted;

	Ticks m_ticksOnStart;
	Ticks m_ticksAccumulated;
};

// win32_hiResTimer.h : 
//
// win32_utilities

#pragma once

#include <chrono>
#include "windows.h"

using std::chrono::microseconds;

using Hertz = NamedType< long long, struct Hertz_Parameter >;
using Ticks = NamedType< long long, struct Ticks_Parameter >;

class HiResTimer
{
public:
    HiResTimer( );
    ~HiResTimer( ) {};

    void Start( );
    void Stop( );

	microseconds GetDuration( );
	microseconds TicksToMicroseconds( Ticks const );
	Ticks        MicroSecondsToTicks( microseconds const );

	void BusyWait( microseconds const );

private:
	unsigned long long const MICROSECONDS_TO_SECONDS = microseconds::period::den;

    static Hertz m_frequency;

	bool  m_bStarted;

	Ticks m_ticksOnStart;
	Ticks m_ticksAccumulated;

	Ticks readHiResTimer( ) const;
};

// win32_hiResTimer.h : 
//
// win32_utilities

#pragma once

#include <chrono>
#include "NamedType.h"
#include "MoreTypes.h"

using std::chrono::microseconds;

using Ticks = NamedType< long long, struct Ticks_Parameter >;

class HiResTimer
{
public:
    HiResTimer( );
    ~HiResTimer( ) {};

	void       Start( );
	Ticks      GetTicksTilStart( )     const;
	fMicroSecs GetMicroSecsTilStart( ) const;
	void       Stop( );
	void       Restart( )
	{
		Stop();
		Start();
	}
	microseconds GetDuration( );

	Ticks ReadHiResTimer( ) const;

	microseconds TicksToMicroseconds( Ticks        const ) const;
	fMicroSecs   TicksToMicroSecs   ( Ticks        const ) const;
	Ticks        MicroSecondsToTicks( microseconds const ) const;
	Ticks        MicroSecsToTicks   ( fMicroSecs    const ) const;

	void BusyWait( microseconds const, Ticks & );

private:
	long long const MICROSECONDS_TO_SECONDS { microseconds::period::den };
	float     const fMICROSECS_TO_SECONDS{ static_cast<float>(MICROSECONDS_TO_SECONDS) };

    static Hertz  m_frequency;
	static fHertz m_fFrequency;

	bool  m_bStarted;

	Ticks m_ticksOnStart;
	Ticks m_ticksAccumulated;
};

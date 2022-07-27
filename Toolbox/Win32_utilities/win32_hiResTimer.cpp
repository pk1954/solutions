// win32_HiResTimer.cpp :
//
// win32_utilities

#include "stdafx.h"
#include <limits>     
#include "win32_hiResTimer.h"

import NamedType;

HiResTimer::HiResTimer()
{
    if (m_frequency == 0_Hertz)                    // frequency is constant for given CPU
    {                                              // first time in application the constructor is called
        LARGE_INTEGER value;                       // frequency is acquired and stored for all HiResTimers
        (void)QueryPerformanceFrequency(&value);
        m_frequency  = Hertz(Cast2UnsignedLong(value.QuadPart));
		m_fFrequency = fHertz(static_cast<float>(m_frequency.GetValue()));
    }
}

Ticks HiResTimer::MicroSecondsToTicks(microseconds const time) const
{
	assert(time.count() < LLONG_MAX / m_frequency.GetValue());
	auto ullTime = static_cast<ULONGLONG>(time.count());
	return Ticks((ullTime * m_frequency.GetValue()) / MICROSECONDS_TO_SECONDS); 
}

microseconds HiResTimer::TicksToMicroseconds(Ticks const ticks) const 
{
	assert(ticks.GetValue() < LLONG_MAX / MICROSECONDS_TO_SECONDS);
	auto ullTicks = static_cast<ULONGLONG>(ticks.GetValue());
	microseconds result
	(                                                 
		(ullTicks * MICROSECONDS_TO_SECONDS) // converts from seconds to microseconds.
		/ m_frequency.GetValue()             // multiply *before* division, otherwise
	);                                       // division would truncate too many significant digits

	return result;
}

void HiResTimer::BusyWait(microseconds const us, Ticks & ticks)
{
	Ticks ticksToWait = MicroSecondsToTicks(us);
	if (ticks == Ticks(0))  //-V1051
		ticks = ReadHiResTimer();
	Ticks ticksTarget = ticks + ticksToWait;

	do
	{ 
		ticks = ReadHiResTimer();
	} while (ticks < ticksTarget);
}

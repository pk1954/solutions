// Win32_HiResTimer.cpp :
//
// win32_utilities

module;

#include <limits>     
#include <cassert>     
#include <Windows.h>

module HiResTimer;

import Types;
import SaveCast;

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

Ticks HiResTimer::microSecondsToTicks(microseconds const time)
{
	assert(time.count() < LLONG_MAX / m_frequency.GetValue());
	auto ullTime = static_cast<ULONGLONG>(time.count());
	return Ticks((ullTime * m_frequency.GetValue()) / MICROSECONDS_TO_SECONDS); 
}

microseconds HiResTimer::ticksToMicroseconds(Ticks const ticks)
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

fMicroSecs HiResTimer::TicksToMicroSecs(Ticks const ticks)
{
	return fMicroSecs(ticks.GetValue() * fMICROSECS_TO_SECONDS / m_fFrequency.GetValue());
}

Ticks HiResTimer::MicroSecsToTicks(fMicroSecs const us)
{
	return Ticks(static_cast<long long>((us.GetValue() * m_fFrequency.GetValue()) / fMICROSECS_TO_SECONDS));
}

void HiResTimer::BusyWait(microseconds const us, Ticks & ticks)
{
	Ticks ticksToWait = microSecondsToTicks(us);
	if (ticks == Ticks(0))  //-V1051
		ticks = ReadHiResTimer();
	Ticks ticksTarget = ticks + ticksToWait;

	do
	{ 
		ticks = ReadHiResTimer();
	} while (ticks < ticksTarget);
}

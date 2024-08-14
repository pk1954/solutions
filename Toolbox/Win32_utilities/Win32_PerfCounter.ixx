// Win32_PerfCounter.ixx
//
// win32_utilities

module;

#include <Windows.h>

export module PerfCounter;

import std;
import Types;
import SaveCast;

export using std::chrono::microseconds;

export using Ticks = NamedType<long long, struct Ticks_Parameter>;
export using std::wstring;

export class PerfCounter
{
public:
	static void Initialize()
	{                                // frequency is constant for given CPU
        LARGE_INTEGER value;                       
        (void)QueryPerformanceFrequency(&value);
        m_frequency  = Hertz(Cast2UnsignedLong(value.QuadPart));
		m_fFrequency = fHertz(static_cast<float>(m_frequency.GetValue()));
    }

	static void BusyWait(microseconds const us, Ticks & ticks)
	{
		Ticks ticksToWait = MicrosecondsToTicks(us);
		if (ticks == Ticks(0))  //-V1051
			ticks = Read();
		Ticks ticksTarget = ticks + ticksToWait;

		do
		{ 
			ticks = Read();
		} while (ticks < ticksTarget);
	}

	static fMicroSecs TicksToMicroSecs(Ticks const ticks)
	{
		return fMicroSecs(ticks.GetValue() * fMICROSECS_TO_SECONDS / m_fFrequency.GetValue());
	}

	static Ticks MicroSecsToTicks(fMicroSecs const us)
	{
		return Ticks(static_cast<long long>((us.GetValue() * m_fFrequency.GetValue()) / fMICROSECS_TO_SECONDS));
	}

	static Ticks Read()
	{
		LARGE_INTEGER value;
		(void)QueryPerformanceCounter(&value);
		return Ticks(value.QuadPart);
	}

	static Ticks MicrosecondsToTicks(microseconds const time)
	{
		Assert(time.count() < LLONG_MAX / m_frequency.GetValue());
		auto ullTime = static_cast<ULONGLONG>(time.count());
		return Ticks((ullTime * m_frequency.GetValue()) / MICROSECONDS_TO_SECONDS); 
	}

	static microseconds TicksToMicroseconds(Ticks const ticks)
	{
		Assert(ticks.GetValue() < LLONG_MAX / MICROSECONDS_TO_SECONDS);
		auto ullTicks = static_cast<ULONGLONG>(ticks.GetValue());
		microseconds result
		(                                                 
			(ullTicks * MICROSECONDS_TO_SECONDS) // converts from seconds to microseconds.
			/ m_frequency.GetValue()             // multiply *before* division, otherwise
		);                                       // division would truncate too many significant digits

		return result;
	}

	static wstring Ticks2wstring(Ticks const ticks)
	{
		fMicroSecs us = PerfCounter::TicksToMicroSecs(ticks);
		return Format2wstring(us);
	}

private:

	inline static long long const MICROSECONDS_TO_SECONDS { microseconds::period::den };
	inline static float     const fMICROSECS_TO_SECONDS   { static_cast<float>(MICROSECONDS_TO_SECONDS) };

	inline static Hertz  m_frequency { 0_Hertz };
	inline static fHertz m_fFrequency{ 0.0_fHertz };
};

// win32_hiResTimer.h : 
//
// win32_utilities

#pragma once

#include <assert.h>
#include <chrono>
#include "MoreTypes.h"

import NamedType;

using std::chrono::microseconds;

using Ticks = NamedType< long long, struct Ticks_Parameter >;

class HiResTimer
{
public:
    HiResTimer();
	virtual ~HiResTimer() = default;

	Ticks ReadHiResTimer() const
	{
		LARGE_INTEGER value;
		(void)QueryPerformanceCounter(& value);
		return Ticks(value.QuadPart);
	}

	void Start()
	{
		if (! m_bStarted)
			m_ticksOnStart = ReadHiResTimer();
		m_bStarted = true;
	}

	void Stop()
	{
		if (m_bStarted)
			m_ticksAccumulated += GetTicksTilStart();
		m_bStarted = false;
	}

	void Restart()
	{
		m_ticksAccumulated = Ticks(0);
		m_ticksOnStart = ReadHiResTimer();
		m_bStarted = true;
	}

	Ticks GetTicksTilStart() const
	{
		return ReadHiResTimer() - m_ticksOnStart;
	}

	fMicroSecs GetMicroSecsTilStart() const
	{
		return TicksToMicroSecs(GetTicksTilStart());
	}

	microseconds GetDuration()
	{
		assert(! m_bStarted);

		microseconds result = TicksToMicroseconds(m_ticksAccumulated);
		m_ticksAccumulated = Ticks(0);

		return result;
	}

	void BusyWait(microseconds const, Ticks &);

	fMicroSecs TicksToMicroSecs(Ticks const ticks) const
	{
		return fMicroSecs(ticks.GetValue() * fMICROSECS_TO_SECONDS / m_fFrequency.GetValue());
	}

	Ticks MicroSecsToTicks(fMicroSecs const us) const
	{
		return Ticks(static_cast<long long>((us.GetValue() * m_fFrequency.GetValue()) / fMICROSECS_TO_SECONDS));
	}

private:

	long long const MICROSECONDS_TO_SECONDS { microseconds::period::den };
	float     const fMICROSECS_TO_SECONDS   { static_cast<float>(MICROSECONDS_TO_SECONDS) };

 	bool  m_bStarted         { false };
	Ticks m_ticksOnStart     { Ticks(0) };
	Ticks m_ticksAccumulated { Ticks(0) };

	inline static Hertz  m_frequency  { 0_Hertz };
	inline static fHertz m_fFrequency { 0.0_fHertz};

	microseconds TicksToMicroseconds(Ticks const) const;
	Ticks MicroSecondsToTicks(microseconds const) const;
};

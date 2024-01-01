// Win32_HiResTimer.ixx
//
// win32_utilities

module;

#include <cassert>

export module HiResTimer;

import Types;
import PerfCounter;

export class HiResTimer
{
public:
	void Start()
	{
		if (!m_bRunning)
			m_ticksOnStart = PerfCounter::Read();
		m_bRunning = true;
	}

	void Stop()
	{
		if (m_bRunning)
			m_ticksAccumulated += GetTicksTilStart();
		m_bRunning = false;
	}

	void Restart()
	{
		m_ticksAccumulated = Ticks(0);
		m_ticksOnStart = PerfCounter::Read();
		m_bRunning = true;
	}

	Ticks GetTicksTilStart() const
	{
		return PerfCounter::Read() - m_ticksOnStart;
	}

	fMicroSecs GetMicroSecsTilStart() const
	{
		return PerfCounter::TicksToMicroSecs(GetTicksTilStart());
	}

	microseconds GetDuration()
	{
		assert(!m_bRunning);

		microseconds result = PerfCounter::TicksToMicroseconds(m_ticksAccumulated);
		m_ticksAccumulated = Ticks(0);

		return result;
	}

private:

	inline static long long const MICROSECONDS_TO_SECONDS { microseconds::period::den };
	inline static float     const fMICROSECS_TO_SECONDS   { static_cast<float>(MICROSECONDS_TO_SECONDS) };

	bool  m_bRunning         { false };
	Ticks m_ticksOnStart     { Ticks(0) };
	Ticks m_ticksAccumulated { Ticks(0) };
};

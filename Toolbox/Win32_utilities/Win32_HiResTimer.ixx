// Win32_HiResTimer.ixx
//
// win32_utilities

module;

#include <cassert>
#include <chrono>
#include <Windows.h>

export module HiResTimer;

import Types;

export using std::chrono::microseconds;

export using Ticks = NamedType< long long, struct Ticks_Parameter >;

export class HiResTimer
{
public:
	HiResTimer();
	virtual ~HiResTimer() = default;

	void Start()
	{
		if (!m_bStarted)
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
		assert(!m_bStarted);

		microseconds result = ticksToMicroseconds(m_ticksAccumulated);
		m_ticksAccumulated = Ticks(0);

		return result;
	}

	static void BusyWait(microseconds const, Ticks&);

	static fMicroSecs TicksToMicroSecs(Ticks const);
	static Ticks MicroSecsToTicks(fMicroSecs const);

	static Ticks ReadHiResTimer()
	{
		LARGE_INTEGER value;
		(void)QueryPerformanceCounter(&value);
		return Ticks(value.QuadPart);
	}

private:

	inline static long long const MICROSECONDS_TO_SECONDS { microseconds::period::den };
	inline static float     const fMICROSECS_TO_SECONDS   { static_cast<float>(MICROSECONDS_TO_SECONDS) };

	bool  m_bStarted         { false };
	Ticks m_ticksOnStart     { Ticks(0) };
	Ticks m_ticksAccumulated { Ticks(0) };

	inline static Hertz  m_frequency { 0_Hertz };
	inline static fHertz m_fFrequency{ 0.0_fHertz };

	static microseconds ticksToMicroseconds(Ticks const);
	static Ticks microSecondsToTicks(microseconds const);
};

// Win32_HiResTimer.ixx
//
// win32_utilities

// Use case 1: Single action timer
//
// BeforeAction AfterAction GetSingleActionTicks 

// Use case 2: Accumulated actions timer 
//
// Reset
// Repeat { BeforeAction AfterAction }
// GetAverageActionTicks
//

export module HiResTimer;

import std;
import Types;
import Observable;

export import PerfCounter;

export class HiResTimer : public Observable
{
public:
	void Reset()
	{
		m_ulActionCounter  = 0;
		m_ticksAccMeasured = Ticks(0);
	}

	void AfterAndBeforeAction()
	{
		Ticks ticksNow { PerfCounter::Read() };
		m_ticksAccMeasured += ticksNow - m_ticksBeforeAction;
		m_ticksBeforeAction = ticksNow;
		++m_ulActionCounter;
	}

	void BeforeAction()
	{
		m_ticksBeforeAction = PerfCounter::Read();
	}

	Ticks AfterAction()
	{
		m_ticksSingleAction = PerfCounter::Read() - m_ticksBeforeAction;
		m_ticksAccMeasured += m_ticksSingleAction;
		++m_ulActionCounter;
		return m_ticksSingleAction;
	}

	Ticks GetSingleActionTicks() const
	{
		return m_ticksSingleAction;
	}

	Ticks GetAverageActionTicks() const
	{
		return m_ticksAccMeasured / m_ulActionCounter;
	}

	wstring Average2wstring() const
	{
		return PerfCounter::Ticks2wstring(GetAverageActionTicks());
	}

private:

	Ticks         m_ticksBeforeAction { Ticks(0) };
	Ticks         m_ticksSingleAction { Ticks(0) };
	Ticks         m_ticksAccMeasured  { Ticks(0) };
	unsigned long m_ulActionCounter   { 0 }; 
};

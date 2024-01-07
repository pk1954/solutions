// HiResClockGen.ixx
//
// win32_utilities

export module HiResClockGen;

export import PerfCounter;

export class HiResClockGen
{
public:

	void Reset(Ticks const ticks)
	{
		m_ticksPerCycleNominal = ticks;
		m_ticksNextComputation = PerfCounter::Read();
	}

	void MaxSpeed()
	{
		Reset(Ticks(0));
	}

	bool IsAtMaxSpeed() const
	{
		return m_ticksPerCycleNominal == Ticks(0);
	}

	bool Time4NextAction() const // can be called in busy loop
	{ 
		if (IsAtMaxSpeed())    // 0 means max speed
			return true;
		else 
			return PerfCounter::Read() >= m_ticksNextComputation;
	}

	void Action() 
	{ 
		m_ticksNextComputation += m_ticksPerCycleNominal; 
	}

private:

	Ticks m_ticksPerCycleNominal { Ticks(0) };  // 0 means run at max speed
	Ticks m_ticksNextComputation { Ticks(0) };
};

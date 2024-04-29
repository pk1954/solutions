// SimulationTime.ixx
//
// Toolbox\Utilities

export module SimulationTime;

import Types;

export class SimulationTime
{
public:
	static void Set(fMicroSecs const newVal = 0._MicroSecs)
	{
		m_timestamp = newVal;
	}

	static fMicroSecs Get()
	{
		return m_timestamp;
	}

	static void Tick(fMicroSecs const timeInc)
	{
		m_timestamp += timeInc;
	}

private:

	inline static fMicroSecs m_timestamp { 0._MicroSecs };
};
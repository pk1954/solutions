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
		m_timeStamp = newVal;
	}

	static fMicroSecs Get()
	{
		return m_timeStamp;
	}

	static void Tick(fMicroSecs const timeInc)
	{
		m_timeStamp += timeInc;
	}

private:

	inline static fMicroSecs m_timeStamp { 0._MicroSecs };
};
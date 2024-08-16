// SimulationTime.ixx
//
// Toolbox\Utilities

export module SimulationTime;

import Types;

export class SimuTime : public fMicroSecs
{
public:
	SimuTime()
	{
		SetValue(0.0f);
	}

	SimuTime(fMicroSecs const t)
	{
		SetValue(t.GetValue());
	}
};

export class SimulationTime
{
public:
	static void Set(fMicroSecs const newVal = 0._MicroSecs)
	{
		m_timestamp = newVal;
	}

	static SimuTime Get()
	{
		return m_timestamp;
	}

	static void Tick(fMicroSecs const timeInc)
	{
		m_timestamp += timeInc;
	}

private:

	inline static SimuTime m_timestamp { 0._MicroSecs };
};
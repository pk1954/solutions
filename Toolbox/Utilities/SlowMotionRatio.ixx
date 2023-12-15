// SlowMotionRatio.ixx
//
// Utilities

module;

#include <array>

export module SlowMotionRatio;

import Observable;
import Types;

using std::array;

export class SlowMotionRatio : public Observable
{
public:

	float GetRatio() const
	{
		return m_fRatio;
	}

	bool IncRatio()
	{
		if (m_ratioIndex >= MAX_INDEX)
			return false;
		++m_ratioIndex;
		Reset();
		return true;
	}

	bool DecRatio()
	{
		if (m_ratioIndex <= 0)
			return false;
		--m_ratioIndex;
		Reset();
		return true;
	}

	fMicroSecs RealTime2SimuTime(fMicroSecs const realTime) const
	{
		return realTime / m_fRatio;
	}

	fMicroSecs SimuTime2RealTime(fMicroSecs const simuTime) const
	{
		return simuTime * m_fRatio;
	}

	void Reset()
	{
		m_fRatio = m_ratioVector[m_ratioIndex];
		NotifyAll(true);
	}

	void Set(float const fRatio)
	{
		m_fRatio = fRatio;
		NotifyAll(true);
	}

private:
	static unsigned int const DEFAULT_INDEX =  6;
	static unsigned int const MAX_INDEX     = 18;

	unsigned int m_ratioIndex { DEFAULT_INDEX };   // index to m_ratioVector
	float        m_fRatio     { m_ratioVector[m_ratioIndex] };

	inline static array< float, SlowMotionRatio::MAX_INDEX + 1 > const m_ratioVector
	{
		1.0f,       // 0
		2.0f,       // 1
		5.0f,       // 2
		10.0f,      // 3
		20.0f,      // 4
		50.0f,      // 5
		100.0f,     // 6
		200.0f,     // 7
		500.0f,     // 8
		1000.0f,    // 9
		2000.0f,    // 10
		5000.0f,    // 11
		10000.0f,   // 12
		20000.0f,   // 13
		50000.0f,   // 14
		100000.0f,  // 15
		200000.0f,  // 16
		500000.0f,  // 17
		1000000.0f  // 18
	};
};
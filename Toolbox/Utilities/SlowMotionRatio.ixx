// SlowMotionRatio.ixx
//
// Utilities

module;

#include <array>

export module SlowMotionRatio;

import BoolOp;
import Observable;
import Types;

using std::array;

export class SlowMotionRatio : public Observable
{
public:

	bool IncRatio()
	{
		if (m_ratioIndex >= MAX_INDEX)
			return false;
		++m_ratioIndex;
		m_bMaxSpeed = false;
		NotifyAll(true);
		return true;
	}

	bool DecRatio()
	{
		if (m_bMaxSpeed || (m_ratioIndex <= 0))
			return false;
		--m_ratioIndex;
		NotifyAll(true);
		return true;
	}

	bool MaxSpeed(tBoolOp const op)
	{
		bool const bRes = m_bMaxSpeed;
		ApplyOp(m_bMaxSpeed, op);
		NotifyAll();
		return bRes;
	}

	bool MaxSpeed(bool const b)
	{
		return MaxSpeed(BoolOp(b));
	}

	void SetMeasuredSlowMo(float const fValue) 
	{ 
		m_fMeasuredSlowMo = fValue;  // no NotifyAll!
	}

	bool  MaxSpeed()          const { return m_bMaxSpeed; }
	float GetNominalSlowMo()  const { return m_ratioVector[m_ratioIndex]; }
	float GetMeasuredSlowMo() const { return m_fMeasuredSlowMo; }

private:
	static unsigned int const DEFAULT_INDEX =  6;
	static unsigned int const MAX_INDEX     = 18;

	unsigned int m_ratioIndex      { DEFAULT_INDEX };   // index to m_ratioVector
	float        m_fMeasuredSlowMo { 0.0f };
	bool         m_bMaxSpeed       { false };

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
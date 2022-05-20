// Radian.h
//
// Utilities

#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include "BasicTypes.h"

using Degrees = NamedType<float, struct degrees_Parameter>;

constexpr Degrees operator"" _Degrees(const long double d) { return Degrees(Cast2Float(d)); }

class Radian
{
public:

	Radian()
		: m_value(null_val)
	{}

	Radian(float const r)
		: m_value(r)
	{}

	Radian(Degrees const d)
      : m_value(d.GetValue() * INV_RADIAN_FACTOR)
	{}

	bool operator== (Radian const other) const { return m_value == other.GetValue(); }
	bool operator!= (Radian const other) const { return m_value != other.GetValue(); }    
	bool operator<= (Radian const other) const { return m_value <= other.GetValue(); }
	bool operator<  (Radian const other) const { return m_value <  other.GetValue(); }
	bool operator>= (Radian const other) const { return m_value >= other.GetValue(); }
	bool operator>  (Radian const other) const { return m_value >  other.GetValue(); }

	float GetValue() const { return m_value; }

	Degrees GetDegrees() const { return Degrees(m_value * RADIAN_FACTOR); }

	static Radian NULL_VAL() { return Radian(null_val); }
	static Radian ZERO_VAL() { return Radian(0.0f); }

	Radian& operator+= (Radian const other) 
	{ 
		m_value += other.m_value; 
		normalize();
		return * this; 
	}

	Radian& operator-= (Radian const other) 
	{ 
		m_value -= other.m_value; 
		normalize();
		return * this; 
	}

	//Radian& operator*= (float const f) 
	//{ 
	//	m_value *= f; 
	//	normalize();
	//	return * this; 
	//}

	friend Radian operator+ (Radian const a, Radian const b)
	{ 
		Radian res { a };
		res += b; 
		return res; 
	}

	friend Radian operator- (Radian const a, Radian const b)
	{ 
		Radian res { a };
		res -= b; 
		return res; 
	}

	friend float operator/ (Radian const a, Radian const b)
	{ 
		return a.m_value / b.m_value; 
	}

	//friend Radian operator* (Radian const a, float const f)
	//{ 
	//	Radian res { a.m_value * f };
	//	res.normalize();
	//	return res;
	//}

	Radian operator- () const { Radian res { -m_value }; return res; }
	Radian operator+ () const { Radian res { +m_value }; return res; }

	MicroMeter Cos() const { return MicroMeter(cos(m_value)); } 
	MicroMeter Sin() const { return MicroMeter(sin(m_value)); } 

	Degrees Radian2Degrees() { return Degrees(m_value * RADIAN_FACTOR); }
	
private:
	inline static float const null_val    { (numeric_limits<float>::min)() };

	inline static float const UPPER_LIMIT { static_cast<float>(M_PI) };
	inline static float const LOWER_LIMIT { 0.0f };

	inline static float const RADIAN_FACTOR     { 180.0f/static_cast<float>(M_PI) };
	inline static float const INV_RADIAN_FACTOR { 1.0f/RADIAN_FACTOR };

	float m_value;

	void normalize() 
	{
		while (m_value > UPPER_LIMIT)
			m_value -= UPPER_LIMIT;
		while (m_value < LOWER_LIMIT)
			m_value += UPPER_LIMIT;
	}
};

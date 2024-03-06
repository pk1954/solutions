// NamedType.ixx
//
// Toolbox\Types
//
// Inspired by Jonathan Boccara
// https://www.fluentcpp.com/

module;

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cmath>

export module Types:NamedType;

import Util;

using std::roundf;
using std::wstring;
using std::wostream;
using std::numeric_limits;
using std::is_integral_v;
using std::is_floating_point_v;

export template <typename T>
struct TypeAttribute 
{ 
	static const wstring unit; 
	static const float   factor; 
};

template <typename T>
concept numeric_t = requires(T param)
{
	requires is_integral_v<T> || is_floating_point_v<T>;
};

export template <numeric_t BASE_TYPE, typename Parameter>
class NamedType
{
public:

	NamedType() : m_value(0) {}

	constexpr explicit NamedType(BASE_TYPE const value) : m_value(value) {}

	explicit operator bool() const noexcept { return m_value != 0; }

	constexpr BASE_TYPE const & GetValue() const { return m_value; }

	constexpr NamedType GetAbs     () const { return NamedType(abs(m_value)); }
	constexpr BASE_TYPE GetAbsValue() const { return BASE_TYPE(abs(m_value)); }

	bool operator== (NamedType const other) const { return m_value == other.GetValue(); }
	bool operator!= (NamedType const other) const { return m_value != other.GetValue(); }
	bool operator<= (NamedType const other) const { return m_value <= other.GetValue(); }
	bool operator<  (NamedType const other) const { return m_value <  other.GetValue(); }
	bool operator>= (NamedType const other) const { return m_value >= other.GetValue(); }
	bool operator>  (NamedType const other) const { return m_value >  other.GetValue(); }

	bool IsZero       () const { return m_value == BASE_TYPE(0); };
	bool IsNotZero    () const { return m_value != BASE_TYPE(0); };
	bool IsPositive   () const { return m_value >  BASE_TYPE(0); };
	bool IsNotPositive() const { return m_value <= BASE_TYPE(0); };
	bool IsNegative   () const { return m_value <  BASE_TYPE(0); };
	bool IsNotNegative() const { return m_value >= BASE_TYPE(0); };

	NamedType& operator=  (BASE_TYPE const value) { m_value = value; return * this; }

	NamedType& operator+= (NamedType const other) { m_value += other.GetValue(); return * this; }
	NamedType& operator-= (NamedType const other) { m_value -= other.GetValue(); return * this; }
	NamedType& operator%= (NamedType const other) { m_value %= other.GetValue(); return * this; }

	NamedType& operator+= (BASE_TYPE const other) { m_value += other; return * this; }
	NamedType& operator-= (BASE_TYPE const other) { m_value -= other; return * this; }
	NamedType& operator%= (BASE_TYPE const other) { m_value %= other; return * this; }

	NamedType& operator*= (BASE_TYPE const i) { m_value *= i; return * this; }
	NamedType& operator/= (BASE_TYPE const i) { m_value /= i; return * this; }

	NamedType  operator- () const { NamedType res { -m_value }; return res; }
	NamedType  operator+ () const { NamedType res { +m_value }; return res; }

	NamedType  operator++() { ++m_value; return * this; }
	NamedType  operator--() { --m_value; return * this; }

	NamedType  operator++(int) { NamedType tmp { *this }; operator++(); return tmp; }
	NamedType  operator--(int) { NamedType tmp { *this }; operator--(); return tmp; }

	bool IsCloseToZero()            const { return ::IsCloseToZero(GetValue()); }
	bool IsCloseTo(NamedType other) const { return (*this - other).IsCloseToZero(); }

	friend NamedType Distance(NamedType const & a, NamedType const & b)
	{
		return NamedType(abs(a.m_value - b.m_value));
	}

	friend NamedType operator+ (NamedType const a, NamedType const b)
	{ 
		NamedType res { a };
		res += b; 
		return res; 
	}

	friend NamedType operator- (NamedType const a, NamedType const b)
	{ 
		NamedType res { a };
		res -= b; 
		return res; 
	}

	friend NamedType operator% (NamedType const a, NamedType const b)
	{ 
		NamedType res { a };
		res %= b; 
		return res; 
	}

	friend NamedType operator+ (NamedType const a, BASE_TYPE const b)
	{ 
		NamedType res { a };
		res += b; 
		return res; 
	}

	friend NamedType operator- (NamedType const a, BASE_TYPE const b)
	{ 
		NamedType res { a };
		res -= b; 
		return res; 
	}

	friend NamedType operator* (NamedType const a, BASE_TYPE const i)
	{ 
		NamedType res { a };
		res *= i; 
		return res; 
	}

	friend NamedType operator/ (NamedType const a, BASE_TYPE const i)
	{ 
		NamedType res { a };
		res /= i; 
		return res; 
	}

	friend BASE_TYPE operator/ (BASE_TYPE const a, NamedType const b)
	{ 
		return BASE_TYPE(a / b.GetValue());
	}

	friend BASE_TYPE operator/ (NamedType const a, NamedType const b)
	{ 
		return BASE_TYPE(a.GetValue() / b.GetValue());
	}

	friend wostream & operator<< (wostream & out, NamedType const & param)
	{
		out << L" " << param.GetValue();
		return out;
	}

	static NamedType MAX_VAL()
	{
		static_assert(std::numeric_limits<BASE_TYPE>::is_specialized, "type has no MAX_VAL");
		static NamedType constexpr res { (numeric_limits<BASE_TYPE>::max)() };
		return res;
	}

	static NamedType NULL_VAL()
	{
		static_assert(std::numeric_limits<BASE_TYPE>::is_specialized, "type has no NULL_VAL");
		static NamedType constexpr res { (numeric_limits<BASE_TYPE>::min)() };
		return res;
	}

	static NamedType ZERO_VAL()
	{
		static NamedType constexpr res { BASE_TYPE(0) };
		return res;
	}

	void Set2Null() { * this = NULL_VAL(); }
	void Set2Zero() { * this = ZERO_VAL(); }

	bool IsNull   () const { return * this == NULL_VAL(); };
	bool IsNotNull() const { return * this != NULL_VAL(); };

	bool IsInRange(auto const min, auto const max) const
	{
		return ::IsInRange(*this, min, max);
	}

	NamedType Round(NamedType const raster) const
	{
		BASE_TYPE value { m_value };
		value /= raster.GetValue();
		return NamedType(roundf(value)) * raster.GetValue();
	}

	NamedType Round() const
	{ 
		return NamedType(roundf(m_value));
	}

private:
	BASE_TYPE m_value;
};

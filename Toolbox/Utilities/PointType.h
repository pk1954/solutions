// PointType.h
//
// Utilities

#pragma once

#include <algorithm>  // min/max/abs templates
#include <iostream>
#include <iomanip>
#include <cmath>
#include <compare>
#include "NamedType.h"

using std::abs;
using std::max;
using std::setw;
using std::wostream;

template <typename BASE_TYPE, typename Parameter> 
class PointType
{
public:

	PointType( ) : m_x( BASE_TYPE::NULL_VAL() ), m_y( BASE_TYPE::NULL_VAL() ) { }
	PointType( BASE_TYPE const _b ) : m_x(_b), m_y(_b) {}
	PointType( BASE_TYPE const _x, BASE_TYPE const _y ) : m_x(_x), m_y(_y) {}

	auto operator <=> (const PointType &) const = default;

    PointType const operator+= (PointType const a) { m_x += a.m_x; m_y += a.m_y; return * this; }
    PointType const operator-= (PointType const a) { m_x -= a.m_x; m_y -= a.m_y; return * this; }
    PointType const operator%= (PointType const a) { m_x %= a.m_x; m_y %= a.m_y; return * this; }

	PointType const operator*= (int       const i) { m_x *= i; m_y *= i; return * this; }
	PointType const operator/= (int       const i) { m_x /= i; m_y /= i; return * this; }

	PointType const operator*= (float     const d) { m_x *= d; m_y *= d; return * this; }
	PointType const operator/= (float     const d) { m_x /= d; m_y /= d; return * this; }

	PointType const operator- () const { return PointType { -m_x, -m_y }; };
	PointType const operator+ () const { return PointType { +m_x, +m_y }; };

	BASE_TYPE const GetX() const { return m_x; }
	BASE_TYPE const GetY() const { return m_y; }

	auto const GetXvalue( ) const { return GetX().GetValue(); }
	auto const GetYvalue( ) const { return GetY().GetValue(); }

	static PointType const & NULL_VAL() 
	{ 
		static PointType res { PointType( BASE_TYPE::NULL_VAL() ) }; 
		return res;
	};

	static PointType const & ZERO_VAL() 
	{ 
		static PointType res { PointType( BASE_TYPE::ZERO_VAL() ) }; 
		return res;
	};

    void Set2Zero( ) { * this = ZERO_VAL(); }
    void Set2Null( ) { * this = NULL_VAL(); }

    bool IsNull   ( ) const { return * this == NULL_VAL(); };
    bool IsNotNull( ) const { return * this != NULL_VAL(); };
    bool IsZero   ( ) const { return * this == ZERO_VAL(); };
    bool IsNotZero( ) const { return * this != ZERO_VAL(); };

	friend PointType const operator+ (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res += b; 
		return res; 
	};

	friend PointType const operator- (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res -= b; 
		return res; 
	};

	friend PointType const operator% (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res %= b; 
		return res; 
	};

	friend PointType const operator+ (PointType const a, int const i) 
	{ 
		PointType res { a }; 
		res += i; 
		return res; 
	};

	friend PointType const operator+ (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res += d; 
		return res; 
	};

	friend PointType const operator- (PointType const a, int const i) 
	{ 
		PointType res { a }; 
		res -= i; 
		return res; 
	};

	friend PointType const operator- (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res -= d; 
		return res; 
	};

	friend PointType const operator* (PointType const a,  float const d) 
	{ 
		PointType res { a }; 
		res *= d; 
		return res; 
	};

	friend PointType const operator* (PointType const a,  int const i) 
	{ 
		PointType res { a }; 
		res *= i; 
		return res; 
	};

	friend PointType const operator/ (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res /= d; 
		return res; 
	};

	friend PointType const operator/ (PointType const a, int const i) 
	{ 
		PointType res { a }; 
		res /= i; 
		return res; 
	};

	friend float const DistSquare( PointType const pntA, PointType const pntB )
	{
		PointType delta {pntA - pntB};
		return delta.GetXvalue() * delta.GetXvalue() + delta.GetYvalue() * delta.GetYvalue();
	}

	friend BASE_TYPE const MaxAbsDelta(PointType const pnt) 
	{
		BASE_TYPE xAbs { abs(pnt.GetXvalue()) };
		BASE_TYPE yAbs { abs(pnt.GetYvalue()) };
		return max( xAbs, yAbs );
	}

	friend wostream & operator<< ( wostream & out, PointType const & param )
	{
		out << OPEN_BRACKET 
			<< param.GetX() << SEPARATOR << param.GetY() 
			<< CLOSE_BRACKET;
		return out;
	}

	inline static wchar_t const SEPARATOR     { L'|' };
	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

private:
    BASE_TYPE m_x;
    BASE_TYPE m_y;
};

// PosType: A specialized PointType used to denote the position of something

template <typename BASE_TYPE> 
class PosType: public PointType<BASE_TYPE, struct pos_tag> 
{
public:
	using PointType<BASE_TYPE, struct pos_tag>::PointType;

	PosType( PointType<BASE_TYPE, struct pos_tag> const & p ) 
		: PointType<BASE_TYPE, struct pos_tag>( p )	
	{}
};

// SizeType: A specialized PointType used to denote the size of something

template <typename BASE_TYPE> 
class SizeType: public PointType<BASE_TYPE, struct siz_tag> 
{
public:
	using PointType<BASE_TYPE, struct siz_tag>::PointType;

	SizeType( PointType<BASE_TYPE, struct siz_tag> const & p ) 
		: PointType<BASE_TYPE, struct siz_tag>( p ) 
	{}
};

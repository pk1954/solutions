// PointType.h
//

#pragma once

#include <algorithm>  // min/max/abs templates
#include <iostream>
#include <iomanip>
#include "NamedType.h"

using std::abs;
using std::max;
using std::setw;

template <typename BASE_TYPE, typename Parameter> 
class PointType
{
public:

	PointType( PointType const & src ) : m_x(src.m_x), m_y(src.m_y) {}
	PointType( BASE_TYPE const _b ) : m_x(_b), m_y(_b) {}
	PointType( BASE_TYPE const _x, BASE_TYPE const _y ) : m_x(_x), m_y(_y) {}

    bool      const operator== (PointType const a) const { return (m_x == a.m_x) && (m_y == a.m_y); }
    bool      const operator!= (PointType const a) const { return (m_x != a.m_x) || (m_y != a.m_y); }

    PointType const operator+= (PointType const a) { m_x += a.m_x; m_y += a.m_y; return * this; }
    PointType const operator-= (PointType const a) { m_x -= a.m_x; m_y -= a.m_y; return * this; }
    PointType const operator%= (PointType const a) { m_x %= a.m_x; m_y %= a.m_y; return * this; }

    PointType const operator*= (int const i) { m_x *= i; m_y *= i; return * this; };
    PointType const operator/= (int const i) { m_x /= i; m_y /= i; return * this; }

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

	friend PointType const operator+ (PointType const a, BASE_TYPE const base) 
	{ 
		PointType res { a }; 
		res += base; 
		return res; 
	};

	friend PointType const operator- (PointType const a, BASE_TYPE const base) 
	{ 
		PointType res { a }; 
		res -= base; 
		return res; 
	};

	friend PointType const operator* (PointType const a, long const l) 
	{ 
		PointType res { a }; 
		res *= l; 
		return res; 
	};

	friend PointType const operator/ (PointType const a, long const l) 
	{ 
		PointType res { a }; 
		res /= l; 
		return res; 
	};

	friend BASE_TYPE const MaxAbsDelta(PointType const pnt) 
	{
		BASE_TYPE xAbs { abs(pnt.GetXvalue()) };
		BASE_TYPE yAbs { abs(pnt.GetYvalue()) };
		return max( xAbs, yAbs );
	}

	friend std::wostream & operator<< ( std::wostream & out, PointType const & param )
	{
		out << L"(" << param.GetXvalue() << L"/" << param.GetYvalue() << L")";
		return out;
	}
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

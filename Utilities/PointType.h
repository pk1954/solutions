// PointType.h
//

#pragma once

#include <iostream>

struct x_tag {};
struct y_tag {};

template <typename BASE_TYPE, typename Parameter> 
class PointType
{
	using BASE_X = NamedType< BASE_TYPE, x_tag >;
	using BASE_Y = NamedType< BASE_TYPE, y_tag >;

public:

	PointType( PointType const & src ) : x(src.x), y(src.y) {}
	PointType( BASE_X    const _x, BASE_Y    const _y ) : x(_x), y(_y) {}
	PointType( BASE_TYPE const _x, BASE_TYPE const _y ) : x(_x), y(_y) {}

    bool      const operator== (PointType const a) const { return (x == a.x) && (y == a.y); }
    bool      const operator!= (PointType const a) const { return (x != a.x) || (y != a.y); }

    PointType const operator+= (PointType const a) { x += a.x; y += a.y; return * this; }
    PointType const operator-= (PointType const a) { x -= a.x; y -= a.y; return * this; }
    PointType const operator%= (PointType const a) { x %= a.x; y %= a.y; return * this; }

    PointType const operator+= (BASE_TYPE const b) { x += BASE_X(b); y += BASE_Y(b); return * this; }
    PointType const operator-= (BASE_TYPE const b) { x -= BASE_X(b); y -= BASE_Y(b); return * this; }

    PointType const operator*= (int const i) { x *= i; y *= i; return * this; };
    PointType const operator/= (int const i) { x /= i; y /= i; return * this; }

    PointType const operator- () const { return PointType( -x, -y ); };
    PointType const operator+ () const { return PointType( +x, +y ); };

	BASE_X const GetX() const { return x; }
	BASE_Y const GetY() const { return y; }

	auto const GetXvalue( ) const { return x.GetValue().GetValue(); }
	auto const GetYvalue( ) const { return y.GetValue().GetValue(); }

	static PointType const & NULL_VAL() 
	{ 
		static PointType res = PointType( BASE_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL() ); 
		return res;
	};

	static PointType const & ZERO_VAL() 
	{ 
		static PointType res = PointType( BASE_TYPE(0), BASE_TYPE(0) ); 
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
		PointType res(a); 
		res += b; 
		return res; 
	};

	friend PointType const operator- (PointType const a, PointType const b) 
	{ 
		PointType res(a); 
		res -= b; 
		return res; 
	};

	friend PointType const operator% (PointType const a, PointType const b) 
	{ 
		PointType res(a); 
		res %= b; 
		return res; 
	};

	friend PointType const operator+ (PointType const a, BASE_TYPE const base) 
	{ 
		PointType res(a); 
		res += base; 
		return res; 
	};

	friend PointType const operator- (PointType const a, BASE_TYPE const base) 
	{ 
		PointType res(a); 
		res -= base; 
		return res; 
	};

	friend PointType const operator* (PointType const a, long const l) 
	{ 
		PointType res(a); 
		res *= l; 
		return res; 
	};

	friend PointType const operator/ (PointType const a, long const l) 
	{ 
		PointType res(a); 
		res /= l; 
		return res; 
	};

	friend PointType const Max(PointType const a, PointType const b) 
	{ 
		return PointType( std::min(a.GetX(), b.GetX()), std::min(a.GetY(), b.GetY()) ); 
	}

	friend PointType const Min(PointType const a, PointType const b) 
	{ 
		return PointType( std::max(a.GetX(), b.GetX()), std::max(a.GetY(), b.GetY()) ); 
	}

	friend BASE_TYPE const MaxAbsDelta(PointType const pnt) 
	{
		BASE_TYPE xAbs{ std::abs(pnt.GetXvalue()) };
		BASE_TYPE yAbs{ std::abs(pnt.GetYvalue()) };
		return BASE_TYPE( std::max( xAbs, yAbs ) );
	}

	friend std::wostream & operator<< ( std::wostream & out, PointType const & param )
	{
		out << L" " << param.GetX() << L" " << param.GetY() << L" ";
		return out;
	}
private:
    BASE_X x;
    BASE_Y y;
};

template <typename T> struct point_traits;

struct pos_tag {};
struct siz_tag {};

// PosType: A specialized PointType used to denote the position of something

template <typename BASE_TYPE> 
class PosType: public PointType<BASE_TYPE, pos_tag> 
{
public:
	using PointType::PointType;

	PosType( PointType const & p ) : PointType( p )	{}

	using point_traits = pos_tag;
};

// SizeType: A specialized PointType used to denote the size of something

template <typename BASE_TYPE> 
class SizeType: public PointType<BASE_TYPE, siz_tag> 
{
public:
	using PointType::PointType;

	SizeType( PointType const & p ) : PointType( p ) {}

	using point_traits = siz_tag;
};

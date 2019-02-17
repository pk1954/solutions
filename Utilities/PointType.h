// PointType.h
//

#pragma once

#include <iostream>

template <typename T> struct point_traits;

template <typename BASE_TYPE, typename Parameter> 
class PointType
{
public:

	PointType( PointType const & src ) : x(src.x), y(src.y) {}
	PointType( BASE_TYPE const _x, BASE_TYPE const _y ) : x(_x), y(_y) {}

    bool      const operator== (PointType const a) const { return (x == a.x) && (y == a.y); }
    bool      const operator!= (PointType const a) const { return (x != a.x) || (y != a.y); }

    PointType const operator+= (PointType const a) { x += a.x; y += a.y; return * this; }
    PointType const operator-= (PointType const a) { x -= a.x; y -= a.y; return * this; }
    PointType const operator%= (PointType const a) { x %= a.x; y %= a.y; return * this; }

    PointType const operator+= (BASE_TYPE const l) { x += l; y += l; return * this; }
    PointType const operator-= (BASE_TYPE const l) { x -= l; y -= l; return * this; }

    PointType const operator*= (int const i) { x *= i; y *= i; return * this; };
    PointType const operator/= (int const i) { x /= i; y /= i; return * this; }

    PointType const operator- () const { return PointType( -x, -y ); };
    PointType const operator+ () const { return PointType( +x, +y ); };

	BASE_TYPE const GetX() const { return x; }
	BASE_TYPE const GetY() const { return y; }

	auto const GetXvalue( ) const { return x.GetValue(); }
	auto const GetYvalue( ) const { return y.GetValue(); }

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

	friend PointType const abs_value(PointType const a) 
	{ 
		return PointType( abs_value(a.GetX()), abs_value(a.GetY()) ); 
	}

	friend BASE_TYPE const MaxAbsDelta(PointType const pnt) 
	{
		return BASE_TYPE( std::max( abs_value(pnt.GetX()), abs_value(pnt.GetY()) ) );
	}

	friend std::wostream & operator<< ( std::wostream & out, PointType const & param )
	{
		out << L" " << param.GetX() << L" " << param.GetY() << L" ";
		return out;
	}
private:
    BASE_TYPE x;
    BASE_TYPE y;
};

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

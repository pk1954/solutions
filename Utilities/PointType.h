// PointType.h
//

#pragma once

template <typename BASE_TYPE> class PointType
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

	PointType const abs_value() { return PointType( x.abs_value(), y.abs_value() ); }

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

private:
    BASE_TYPE x;
    BASE_TYPE y;
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator+ (PointType<BASE_TYPE> const a, PointType<BASE_TYPE> const b) 
{ 
	PointType<BASE_TYPE> res(a); 
	res += b; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator- (PointType<BASE_TYPE> const a, PointType<BASE_TYPE> const b) 
{ 
	PointType<BASE_TYPE> res(a); 
	res -= b; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator% (PointType<BASE_TYPE> const a, PointType<BASE_TYPE> const b) 
{ 
	PointType<BASE_TYPE> res(a); 
	res %= b; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator+ (PointType<BASE_TYPE> const a, BASE_TYPE const base) 
{ 
	PointType<BASE_TYPE> res(a); 
	res += base; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator- (PointType<BASE_TYPE> const a, BASE_TYPE const base) 
{ 
	PointType<BASE_TYPE> res(a); 
	res -= base; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator* (PointType<BASE_TYPE> const a, long const l) 
{ 
	PointType<BASE_TYPE> res(a); 
	res *= l; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const operator/ (PointType<BASE_TYPE> const a, long const l) 
{ 
	PointType<BASE_TYPE> res(a); 
	res /= l; 
	return res; 
};

template <typename BASE_TYPE>
PointType<BASE_TYPE> const Max(PointType<BASE_TYPE> const a, PointType<BASE_TYPE> const b) 
{ 
	return PointType<BASE_TYPE>( std::min(a.GetX(), b.GetX()), std::min(a.GetY(), b.GetY()) ); 
}

template <typename BASE_TYPE>
PointType<BASE_TYPE> const Min(PointType<BASE_TYPE> const a, PointType<BASE_TYPE> const b) 
{ 
	return PointType<BASE_TYPE>( std::max(a.GetX(), b.GetX()), std::max(a.GetY(), b.GetY()) ); 
}

template <typename BASE_TYPE>
BASE_TYPE const MaxAbsDelta(PointType<BASE_TYPE> const pnt) 
{
    return BASE_TYPE( std::max( pnt.GetX().abs_value(), pnt.GetY().abs_value() ) );
}

template <typename BASE_TYPE>
std::wostream & operator<< ( std::wostream & out, PointType<BASE_TYPE> const & param )
{
    out << L" " << param.GetX() << L" " << param.GetY() << L" ";
    return out;
}

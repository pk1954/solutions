// PointType.h
//

#pragma once

template <class BASE_TYPE> class PointType
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

    PointType operator- () const { return PointType( -x, -y ); };

	BASE_TYPE const GetX() const { return x; }
	BASE_TYPE const GetY() const { return y; }

	static PointType const & NULL_VAL() 
	{ 
		static PointType res = PointType( BASE_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL() ); 
		return res;
	};

    void Set2Null( ) { * this = NULL_VAL(); }

    bool IsNull   ( ) const { return * this == NULL_VAL(); };
    bool IsNotNull( ) const { return * this != NULL_VAL(); };

private:
    BASE_TYPE x;
    BASE_TYPE y;
};


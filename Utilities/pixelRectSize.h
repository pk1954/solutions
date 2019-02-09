// pixelRectSize.h
//

#pragma once

#include "pixel.h"
// #include "PointType.h"

//using PixelRectSize = PointType< PIXEL >;

class PixelRectSize
{
public:
    PixelRectSize( PIXEL const pixWidth, PIXEL const pixHeight ) : x(pixWidth), y(pixHeight) {};

    bool const operator== (PixelRectSize const a) const { return (x == a.x) && (y == a.y); }
    bool const operator!= (PixelRectSize const a) const { return (x != a.x) || (y != a.y); }

    PixelRectSize const operator*= (long const l) { x *= l; y *= l; return *this; };
    PixelRectSize const operator/= (long const l) { x /= l; y /= l; return *this; };

    PixelRectSize const operator+= (PIXEL const p) { x += p; y += p; return *this; };
    PixelRectSize const operator-= (PIXEL const p) { x -= p; y -= p; return *this; };

    PIXEL GetX( ) const { return x;  }
    PIXEL GetY( ) const { return y; }

	long const GetXvalue( ) const { return x.GetValue(); }
	long const GetYvalue( ) const { return y.GetValue(); }

    void Set2Zero( ) { * this = ZERO_VAL(); }
    bool IsZero( ) const { return * this == ZERO_VAL(); };

	static const PixelRectSize ZERO_VAL()
	{
		PixelRectSize value = PixelRectSize( PIXEL(0_PIXEL), PIXEL(0_PIXEL) );
		return value;
	}

private:
    PIXEL x;
    PIXEL y;
};

inline PixelRectSize const operator* (PixelRectSize const & a, long const l) { PixelRectSize res(a); res *= l; return res; };
inline PixelRectSize const operator/ (PixelRectSize const & a, long const l) { PixelRectSize res(a); res /= l; return res; };

inline PixelRectSize const operator+ (PixelRectSize const & a, PIXEL const p ) { PixelRectSize res(a); res += p; return res; };
inline PixelRectSize const operator- (PixelRectSize const & a, PIXEL const p ) { PixelRectSize res(a); res -= p; return res; };

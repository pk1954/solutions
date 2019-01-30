// pixelPoint.h
//

#pragma once

#include <algorithm>   // max
#include <stdlib.h>    // abs
#include "util.h"     
#include "debug.h"
#include "pixel.h"

class PixelPoint
{
public:
	         PixelPoint( )                                : x( 0), y( 0) {}
    explicit PixelPoint( PIXEL const _a )                 : x(_a), y(_a) {}
             PixelPoint( PIXEL const _x, PIXEL const _y ) : x(_x), y(_y) {};

    bool       const operator== ( PixelPoint const a ) const { return ( x == a.x ) && ( y == a.y ); };
    bool       const operator!= ( PixelPoint const a ) const { return ( x != a.x ) || ( y != a.y ); };

    PixelPoint const operator+= ( PixelPoint const a ) { x += a.x; y += a.y; return *this; };
    PixelPoint const operator-= ( PixelPoint const a ) { x -= a.x; y -= a.y; return *this; };

    PixelPoint const operator+= ( PIXEL const p ) { x += p; y += p; return *this; };
    PixelPoint const operator-= ( PIXEL const p ) { x -= p; y -= p; return *this; };

    PixelPoint const operator*= (long const l) { x *= l; y *= l; return *this; };
    PixelPoint const operator/= (long const l) { x /= l; y /= l; return *this; };

    PixelPoint operator- () const { return PixelPoint( -x, -y ); };

	PIXEL const GetX() const { return x; }
	PIXEL const GetY() const { return y; }

	long const GetXlong() const { return x.GetValue(); }
	long const GetYlong() const { return y.GetValue(); }

	void SetX( PIXEL const value ) { x = value; }
	void SetY( PIXEL const value ) { y = value; }

	void IncX( PIXEL const value ) { x += value; }
	void IncY( PIXEL const value ) { y += value; }

	void DecX( PIXEL const value ) { x -= value; }
	void DecY( PIXEL const value ) { y -= value; }

	static const PixelPoint PIXEL_POINT_ZERO()
	{
		PixelPoint value = PixelPoint( PIXEL(0_PIXEL), PIXEL(0_PIXEL) );
		return value;
	}

private:
	PIXEL x;
    PIXEL y;
};

inline PixelPoint const operator+ (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator+ (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res += p; return res; };
inline PixelPoint const operator- (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res -= p; return res; };

inline PixelPoint const operator* (PixelPoint const a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const a, long const l) { PixelPoint res(a); res /= l; return res; };

inline PixelPoint const abs( PixelPoint const a ) { return PixelPoint( ::abs(a.GetX()), ::abs(a.GetY() ) ); }

inline PIXEL const MaxAbsDelta(PixelPoint const pp) 
{
    return PIXEL( max( abs( pp.GetXlong() ), abs( pp.GetYlong() ) ) );
}

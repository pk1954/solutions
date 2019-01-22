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
    PIXEL x;
    PIXEL y;

	PixelPoint( )                                : x(0), y(0)   {}
    PixelPoint( PIXEL const _a )                 : x(_a), y(_a) {}
    PixelPoint( PIXEL const _x, PIXEL const _y ) : x(_x), y(_y) {};

    bool       const operator== ( PixelPoint const a ) const { return ( x == a.x ) && ( y == a.y ); };
    bool       const operator!= ( PixelPoint const a ) const { return ( x != a.x ) || ( y != a.y ); };

    PixelPoint const operator+= ( PixelPoint const a ) { x += a.x; y += a.y; return *this; };
    PixelPoint const operator-= ( PixelPoint const a ) { x -= a.x; y -= a.y; return *this; };

    PixelPoint const operator*= (long const l) { x *= l; y *= l; return *this; };
    PixelPoint const operator/= (long const l) { x /= l; y /= l; return *this; };

    PixelPoint operator- () const { return PixelPoint( -x, -y ); };
};

inline PixelPoint const operator+ (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator* (PixelPoint const a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const a, long const l) { PixelPoint res(a); res /= l; return res; };

inline PixelPoint const abs( PixelPoint const a ) { return PixelPoint( ::abs(a.x), ::abs(a.y ) ); }

inline PIXEL const MaxAbsDelta(PixelPoint const pp) 
{
    return PIXEL( max( abs( pp.x.get() ), abs( pp.y.get() ) ) );
}

// pixelPoint.h
//

#pragma once

#include <stdlib.h>    // abs
#include "util.h"      // sgn

class PixelPoint
{
public:
    long x;
    long y;

    PixelPoint()                               { x =    0; y =    0; };
    PixelPoint( long const _x, long const _y ) { x =   _x; y =   _y; };

    bool       const operator== ( PixelPoint const & a ) const { return ( x == a.x ) && ( y == a.y ); };
    bool       const operator!= ( PixelPoint const & a ) const { return ( x != a.x ) || ( y != a.y ); };

    PixelPoint const operator+= ( PixelPoint const & a ) { x += a.x; y += a.y; return *this; };
    PixelPoint const operator-= ( PixelPoint const & a) { x -= a.x; y -= a.y; return *this; };

    PixelPoint const operator+= (long const l) { x += l; y += l; return *this; };
    PixelPoint const operator-= (long const l) { x -= l; y -= l; return *this; };
    PixelPoint const operator*= (long const l) { x *= l; y *= l; return *this; };
    PixelPoint const operator/= (long const l) { x /= l; y /= l; return *this; };

    PixelPoint const operator+= (short const s) { x += s; y += s; return *this; };
    PixelPoint const operator-= (short const s) { x -= s; y -= s; return *this; };
    PixelPoint const operator*= (short const s) { x *= s; y *= s; return *this; };
    PixelPoint const operator/= (short const s) { x /= s; y /= s; return *this; };

    PixelPoint operator- () const { return PixelPoint( -x, -y ); };
};

inline PixelPoint const operator+ (PixelPoint const & a, PixelPoint const & b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const & a, PixelPoint const & b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator* (PixelPoint const & a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const & a, long const l) { PixelPoint res(a); res /= l; return res; };
inline PixelPoint const operator+ (PixelPoint const & a, long const l) { PixelPoint res(a); res += l; return res; };
inline PixelPoint const operator- (PixelPoint const & a, long const l) { PixelPoint res(a); res -= l; return res; };

inline PixelPoint const abs( PixelPoint const & a ) { return PixelPoint( ::abs(a.x), ::abs(a.y ) ); }
inline PixelPoint const sgn( PixelPoint const & a ) { return PixelPoint( ::sgn(a.x), ::sgn(a.y ) ); }

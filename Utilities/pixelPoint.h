// pixelPoint.h
//

#pragma once

class PixelPoint
{
public:
    long x;
    long y;

    PixelPoint()                               { x =    0; y =    0; };
    PixelPoint( long const _x, long const _y ) { x =   _x; y =   _y; };

    bool       const operator== ( PixelPoint const & a ) { return ( x == a.x ) && ( y == a.y ); };
    bool       const operator!= ( PixelPoint const & a ) { return ( x != a.x ) || ( y != a.y ); };

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

    PixelPoint const operator*= ( PixelPoint const p ) { x *= p.x; y *= p.y; return *this; };
};

inline PixelPoint const operator+ (PixelPoint const & a, PixelPoint const & b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const & a, PixelPoint const & b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator* (PixelPoint const & a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const & a, long const l) { PixelPoint res(a); res /= l; return res; };
inline PixelPoint const operator+ (PixelPoint const & a, long const l) { PixelPoint res(a); res += l; return res; };
inline PixelPoint const operator- (PixelPoint const & a, long const l) { PixelPoint res(a); res -= l; return res; };

inline PixelPoint const operator* ( PixelPoint const & a, PixelPoint const & b ) { PixelPoint res( a ); res *= b; return res; };

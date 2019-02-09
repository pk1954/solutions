// kGridPoint.h : 
//
// library Utilities

#pragma once

#include "pixelPoint.h"
#include "pixelRect.h"

class KGridPoint
{
public:
    KGridPoint( long const _x, long const _y ) : x(_x) , y(_y) { };
    explicit KGridPoint( PixelPoint const pxPnt ) : x(pxPnt.GetXvalue()), y(pxPnt.GetYvalue()) { }; 

    bool const operator== (KGridPoint const a) const { return (a.x == x) && (a.y == y); };
    bool const operator!= (KGridPoint const a) const { return (a.x != x) || (a.y != y); };

    KGridPoint const operator*= (short const s) { x *= s; y *= s; return *this; };
    KGridPoint const operator/= (short const s) { x /= s; y /= s; return *this; };

    long GetX( ) const { return x; };
    long GetY( ) const { return y; };

    static KGridPoint const KGRID_POINT_EMPTY;

private:
    long x;
    long y;
};

inline KGridPoint const operator* (KGridPoint const a, short const s) { KGridPoint res(a); res *= s; return res; };
inline KGridPoint const operator/ (KGridPoint const a, short const s) { KGridPoint res(a); res /= s; return res; };

KGridPoint Pixel2KGridSize( PixelPoint const, PIXEL const );
PixelPoint KGrid2PixelSize( KGridPoint const, PIXEL const );

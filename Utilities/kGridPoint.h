// kGridPoint.h : 
//

#pragma once

#include "pixelPoint.h"
#include "pixelRectSize.h"

class KGridPoint
{
public:
    KGridPoint( long const _x, long const _y ) : x(_x) , y(_y) { };
    explicit KGridPoint( PixelPoint const pxPnt ) : x(pxPnt.x.get()), y(pxPnt.y.get()) { }; 

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

class KGridRect
{
public:
    KGridRect( KGridPoint const kpPos, KGridPoint const kpSize ) : m_pos(kpPos), m_size(kpSize) {};

    bool const operator== (KGridRect const &a) const { return (a.m_pos == m_pos) && (a.m_size == m_size); };
    bool const operator!= (KGridRect const &a) const { return (a.m_pos != m_pos) || (a.m_size != m_size); };

    KGridPoint const GetPos ( ) const { return m_pos;  };
    KGridPoint const GetSize( ) const { return m_size; };

    static KGridRect const KGRID_RECT_EMPTY;

    bool const IsEmpty( )    const { return ( *this == KGRID_RECT_EMPTY ); }
    bool const IsNotEmpty( ) const { return ( *this != KGRID_RECT_EMPTY ); }

private:
    KGridPoint m_pos;
    KGridPoint m_size;
};

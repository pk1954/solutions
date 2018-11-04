// kGridPoint.h : 
//

#pragma once

#include "pixelPoint.h"

class KGridPoint
{
public:
    KGridPoint( long const _x, long const _y ) : x(_x) , y(_y) { };
    explicit KGridPoint( PixelPoint const pxPnt ) : x(pxPnt.x), y(pxPnt.y) { }; 

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

KGridPoint Pixel2KGridSize( PixelPoint const, short );
PixelPoint KGrid2PixelSize( KGridPoint const, short );

class KGridRect
{
public:
    KGridRect( KGridPoint const kpPos, KGridPoint const kpSize ) : m_pos(kpPos), m_siz(kpSize) {};

    bool const operator== (KGridRect const &a) const { return (a.m_pos == m_pos) && (a.m_siz == m_siz); };
    bool const operator!= (KGridRect const &a) const { return (a.m_pos != m_pos) || (a.m_siz != m_siz); };

    KGridPoint const GetPos( ) const { return m_pos; };
    KGridPoint const GetSiz( ) const { return m_siz; };

    static KGridRect const KGRID_RECT_EMPTY;

    bool const IsEmpty( )    const { return ( *this == KGRID_RECT_EMPTY ); }
    bool const IsNotEmpty( ) const { return ( *this != KGRID_RECT_EMPTY ); }

private:
    KGridPoint m_pos;
    KGridPoint m_siz;
};

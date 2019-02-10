// kGridPoint.cpp
//

#include "stdafx.h"
#include "kGridPoint.h"

static long const KGRID_FACTOR = 1024;

KGridPoint Pixel2KGridSize( PixelPoint const ptSize, PIXEL const pixFieldSize )
{
	PixelPoint size( ptSize * KGRID_FACTOR / pixFieldSize.GetValue() );
    return KGridPoint( size.GetXvalue(), size.GetYvalue() );
}

PixelPoint KGrid2PixelSize( KGridRectSize const kgpSize, PIXEL const pixFieldSize ) 
{
    KGridRectSize size = (kgpSize * CastToShort(pixFieldSize.GetValue())) / KGRID_FACTOR;
    return PixelPoint( PIXEL(size.GetX()), PIXEL(size.GetY()) );
}

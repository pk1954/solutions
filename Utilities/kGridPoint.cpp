// kGridPoint.cpp
//

#include "stdafx.h"
#include "util.h"     
#include "kGridPoint.h"

KGridPoint const KGridPoint::KGRID_POINT_EMPTY( 0, 0 );

static long const KGRID_FACTOR = 1024;

KGridPoint Pixel2KGridSize( PixelPoint const ptSize, PIXEL const pixFieldSize )
{
    return KGridPoint( ptSize * KGRID_FACTOR / pixFieldSize.GetValue() );
}

PixelPoint KGrid2PixelSize( KGridPoint const kgpSize, PIXEL const pixFieldSize ) 
{
    KGridPoint kgp = (kgpSize * CastToShort(pixFieldSize.GetValue())) / KGRID_FACTOR;
    return PixelPoint( PIXEL(kgp.GetX()), PIXEL(kgp.GetY()) );
}

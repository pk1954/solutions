// kGridPoint.cpp
//

#include "stdafx.h"
#include "kGridPoint.h"

KGridPoint const KGridPoint::KGRID_POINT_EMPTY( 0, 0 );
KGridRect  const KGridRect ::KGRID_RECT_EMPTY ( KGridPoint::KGRID_POINT_EMPTY, KGridPoint::KGRID_POINT_EMPTY );

static long const KGRID_FACTOR = 1024;

KGridPoint Pixel2KGridSize( PixelPoint const ptSize, short const sFieldSize )
{
    PixelPoint pixSize = ptSize * KGRID_FACTOR;
    pixSize /= sFieldSize;
    return KGridPoint( pixSize );
}

PixelPoint KGrid2PixelSize( KGridPoint const kgpSize, short const sFieldSize ) 
{
    KGridPoint kgp = (kgpSize * sFieldSize);
    kgp /= KGRID_FACTOR;
    return PixelPoint( kgp.GetX(), kgp.GetY() );
}

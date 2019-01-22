// kGridPoint.cpp
//

#include "stdafx.h"
#include "kGridPoint.h"

KGridPoint const KGridPoint::KGRID_POINT_EMPTY( 0, 0 );

static long const KGRID_FACTOR = 1024;

KGridPoint Pixel2KGridSize( PixelPoint const ptSize, PIXEL const pixFieldSize )
{
    PixelPoint pixSize = ptSize * KGRID_FACTOR;
    pixSize /= pixFieldSize.get();
    return KGridPoint( pixSize.x.get(), pixSize.y.get() );
}

PixelPoint KGrid2PixelSize( KGridPoint const kgpSize, PIXEL const pixFieldSize ) 
{
    KGridPoint kgp = (kgpSize * CastToShort(pixFieldSize.get()));
    kgp /= KGRID_FACTOR;
    return PixelPoint( PIXEL(kgp.GetX()), PIXEL(kgp.GetY()) );
}

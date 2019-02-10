// kGridPoint.h : 
//
// library Utilities

#pragma once

#include "PixelTypes.h"
#include "PointType.h"

using KGridPoint    = PointType< long, struct KGridPoint_Parameter >;
using KGridRectSize = PointType< long, struct PixelRectSize_Parameter >;

KGridPoint Pixel2KGridSize( PixelPoint    const, PIXEL const );
PixelPoint KGrid2PixelSize( KGridRectSize const, PIXEL const );

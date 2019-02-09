// pixelRect.h
//

#pragma once

#include <assert.h>
#include "pixel.h"
#include "pixelPoint.h"
#include "RectType.h"

using PixelRectSize = PointType< PIXEL,                            struct PixelRectSize_Parameter >;
using PixelRect     = RectType < PIXEL, PixelPoint, PixelRectSize, struct PixelRect_Parameter >;

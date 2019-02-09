// PixelTypes.h
//

#pragma once

#include "util.h"
#include "NamedType.h"
#include "PointType.h"
#include "RectType.h"

using PIXEL         = NamedType< long,                             struct PIXEL_Parameter >;
using PixelPoint    = PointType< PIXEL,                            struct PixelPoint_Parameter >;
using PixelRectSize = PointType< PIXEL,                            struct PixelRectSize_Parameter >;
using PixelRect     = RectType < PIXEL, PixelPoint, PixelRectSize, struct PixelRect_Parameter >;

constexpr PIXEL operator"" _PIXEL( unsigned long long ull )
{
	return PIXEL( CastToShort( ull ) );
}

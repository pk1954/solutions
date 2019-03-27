// PixelTypes.h
//

#pragma once

#include "util.h"
#include "NamedType.h"
#include "PointType.h"
#include "RectType.h"

using PIXEL         = NamedType< long, struct PIXEL_Parameter >;
using PixelPoint    = PosType  < PIXEL >;
using PixelRectSize = SizeType < PIXEL >;
using PixelRect     = RectType < PIXEL >;

constexpr PIXEL operator"" _PIXEL( unsigned long long ull )
{
	return PIXEL( CastToShort( ull ) );
}

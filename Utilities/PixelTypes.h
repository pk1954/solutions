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

using PIXEL_X = NamedType<PIXEL, x_tag>;
using PIXEL_Y = NamedType<PIXEL, y_tag>;

constexpr PIXEL_X operator"" _PIXEL_X( unsigned long long ull )
{
	return PIXEL_X( PIXEL( CastToLong( ull ) ) );
}

constexpr PIXEL_Y operator"" _PIXEL_Y( unsigned long long ull )
{
	return PIXEL_Y( PIXEL( CastToLong( ull ) ) );
}

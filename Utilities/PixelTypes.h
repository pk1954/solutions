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

static PixelPoint const PP_NULL( PixelPoint::NULL_VAL() );   // compiler generates call!
static PixelPoint const PP_ZERO( PixelPoint::ZERO_VAL() );   // compiler generates call!

constexpr PIXEL operator"" _PIXEL( unsigned long long ull )
{
	return PIXEL( CastToShort( ull ) );
}

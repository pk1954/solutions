// PixelTypes.h
//
// Utilities

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

using fPIXEL         = NamedType< double, struct fPIXEL_Parameter >;
using fPixelPoint    = PosType  < fPIXEL >;
using fPixelRectSize = SizeType < fPIXEL >;
using fPixelRect     = RectType < fPIXEL >;

static fPixelPoint const fPP_NULL( fPixelPoint::NULL_VAL() );   // compiler generates call!
static fPixelPoint const fPP_ZERO( fPixelPoint::ZERO_VAL() );   // compiler generates call!

constexpr fPIXEL operator"" _fPIXEL( long double ld )
{
	return fPIXEL( static_cast<double>( ld ) );
}

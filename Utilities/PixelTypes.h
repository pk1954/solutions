// PixelTypes.h
//
// Utilities

#pragma once

#include "util.h"
#include "NamedType.h"
#include "MoreTypes.h"
#include "PointType.h"
#include "RectType.h"

/////////////////// PIXEL ///////////////////////////////////

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

/////////////////// fPIXEL ///////////////////////////////////

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

/////////////////// conversions ///////////////////////////////////

static fPIXEL convert2fPIXEL( PIXEL const pixel )
{
	return fPIXEL( static_cast<double>( pixel.GetValue() ) );
}

static fPixelPoint convert2fPixelPoint( PixelPoint const pixPoint )
{
	return fPixelPoint( convert2fPIXEL( pixPoint.GetX() ), convert2fPIXEL( pixPoint.GetY() ) );
}

static fPixelRectSize convert2fPixelRectSize( PixelRectSize const rectSize )
{
	return fPixelRectSize( convert2fPIXEL( rectSize.GetX() ), convert2fPIXEL( rectSize.GetY() ) );
}

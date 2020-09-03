// PixelTypes.h
//
// Utilities

#pragma once

#include "util.h"
#include "NamedType.h"
#include "MoreTypes.h"
#include "PointType.h"
#include "RectType.h"
#include "CircleType.h"

/////////////////// PIXEL ///////////////////////////////////

using PIXEL         = NamedType  < long, struct PIXEL_Parameter >;
using PixelPoint    = PosType    < PIXEL >;
using PixelRectSize = SizeType   < PIXEL >;
using PixelRect     = RectType   < PIXEL >;
using PixelCircle   = CircleType < PIXEL >;

static PixelPoint const PP_NULL( PixelPoint::NULL_VAL() );   // compiler generates call!
static PixelPoint const PP_ZERO( PixelPoint::ZERO_VAL() );   // compiler generates call!

constexpr PIXEL operator"" _PIXEL( unsigned long long ull )
{
	return PIXEL( CastToShort( ull ) );
}

/////////////////// fPIXEL ///////////////////////////////////

using fPIXEL         = NamedType  < float, struct fPIXEL_Parameter >;
using fPixelPoint    = PosType    < fPIXEL >;
using fPixelRectSize = SizeType   < fPIXEL >;
using fPixelRect     = RectType   < fPIXEL >;
using fPixelCircle   = CircleType < fPIXEL >;

static fPixelPoint const fPP_NULL( fPixelPoint::NULL_VAL() );   // compiler generates call!
static fPixelPoint const fPP_ZERO( fPixelPoint::ZERO_VAL() );   // compiler generates call!

constexpr fPIXEL operator"" _fPIXEL( long double ld )
{
	return fPIXEL( CastToFloat( ld ) );
}

inline static fPIXEL Hypot( fPixelPoint const pt ) 
{ 
	return fPIXEL( std::hypotf(pt.GetXvalue(), pt.GetYvalue() ) );
};

inline static fPixelPoint OrthoVector( fPixelPoint const & vect, fPIXEL const width )
{
	fPIXEL fHypot = Hypot( vect );
	assert( ! IsCloseToZero( fHypot.GetValue() ) );
	return fPixelPoint( vect.GetY(), - vect.GetX() ) * (width / fHypot);
}

inline bool IsCloseToZero( fPixelPoint const vect )
{
	return IsCloseToZero( vect.GetXvalue() ) && IsCloseToZero( vect.GetYvalue() );
}

/////////////////// conversions ///////////////////////////////////

static PIXEL Convert2PIXEL( fPIXEL const fPixel )
{
	return PIXEL( CastToLong( fPixel.GetValue() ) );
}

static PixelPoint Convert2PixelPoint( fPixelPoint const fPixPoint )
{
	return PixelPoint( Convert2PIXEL( fPixPoint.GetX() ), Convert2PIXEL( fPixPoint.GetY() ) );
}

static PixelRectSize Convert2PixelRectSize( fPixelRectSize const fRectSize )
{
	return PixelRectSize( Convert2PIXEL( fRectSize.GetX() ), Convert2PIXEL( fRectSize.GetY() ) );
}

static fPIXEL Convert2fPIXEL( PIXEL const pixel )
{
	return fPIXEL( CastToFloat( pixel.GetValue() ) );
}

static fPixelPoint Convert2fPixelPoint( PixelPoint const pixPoint )
{
	return fPixelPoint( Convert2fPIXEL( pixPoint.GetX() ), Convert2fPIXEL( pixPoint.GetY() ) );
}

static fPixelRectSize Convert2fPixelRectSize( PixelRectSize const rectSize )
{
	return fPixelRectSize( Convert2fPIXEL( rectSize.GetX() ), Convert2fPIXEL( rectSize.GetY() ) );
}

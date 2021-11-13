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
#include "EllipseType.h"

/////////////////// PIXEL ///////////////////////////////////

using PIXEL         = NamedType  < long, struct PIXEL_Parameter >;
using PixelPoint    = PosType    < PIXEL >;
using PixelRectSize = SizeType   < PIXEL >;
using PixelRect     = RectType   < PIXEL >;
using PixelCircle   = CircleType < PIXEL >;
using PixelEllipse  = EllipseType< PIXEL >;

inline static PixelPoint const PP_NULL(PixelPoint::NULL_VAL());   // compiler generates call!
inline static PixelPoint const PP_ZERO(PixelPoint::ZERO_VAL());   // compiler generates call!

constexpr PIXEL operator"" _PIXEL(unsigned long long ull)
{
	return PIXEL(Cast2Short(ull));
}

/////////////////// fPixel ///////////////////////////////////

using fPixel         = NamedType   < float, struct fPixel_Parameter >;
using fPixelPoint    = PosType     < fPixel >;
using fPixelRectSize = SizeType    < fPixel >;
using fPixelRect     = RectType    < fPixel >;
using fPixelCircle   = CircleType  < fPixel >;
using fPixelEllipse  = EllipseType < fPixel >;

inline static fPixelPoint const fPP_NULL(fPixelPoint::NULL_VAL());   // compiler generates call!
inline static fPixelPoint const fPP_ZERO(fPixelPoint::ZERO_VAL());   // compiler generates call!

constexpr fPixel operator"" _fPixel(long double ld)
{
	return fPixel(Cast2Float(ld));
}

/////////////////// conversions ///////////////////////////////////

static PIXEL Convert2PIXEL(fPixel const fPixel)
{
	return PIXEL(Cast2Long(fPixel.GetValue()));
}

static constexpr fPixel Convert2fPixel(PIXEL const pixel)
{
	return fPixel(Cast2Float(pixel.GetValue()));
}

static PixelPoint Convert2PixelPoint(fPixelPoint const fPixPoint)
{
	return PixelPoint(Convert2PIXEL(fPixPoint.GetX()), Convert2PIXEL(fPixPoint.GetY()));
}

static fPixelPoint Convert2fPixelPoint(PixelPoint const pixPoint)
{
	return fPixelPoint(Convert2fPixel(pixPoint.GetX()), Convert2fPixel(pixPoint.GetY()));
}

static PixelRectSize Convert2PixelRectSize(fPixelRectSize const fRectSize)
{
	return PixelRectSize(Convert2PIXEL(fRectSize.GetX()), Convert2PIXEL(fRectSize.GetY()));
}

static fPixelRectSize Convert2fPixelRectSize(PixelRectSize const rectSize)
{
	return fPixelRectSize(Convert2fPixel(rectSize.GetX()), Convert2fPixel(rectSize.GetY()));
}

static fPixelRect Convert2fPixelRect(PixelRect const & rect)
{
	return fPixelRect
		   (
			   Convert2fPixel(rect.GetLeft()),
			   Convert2fPixel(rect.GetTop()),
			   Convert2fPixel(rect.GetRight()),
			   Convert2fPixel(rect.GetBottom())
		  );
}

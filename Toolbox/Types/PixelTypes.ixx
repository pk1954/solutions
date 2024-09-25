// PixelTypes.ixx
//
// Toolbox\Types

export module Types:PixelTypes;

import SaveCast;
import Util;
import :NamedType;
import :CircleType;
import :EllipseType;
import :PointType;
import :LineType;
import :RectType;

/////////////////// PIXEL ///////////////////////////////////

export using PIXEL         = NamedType  < long, struct PIXEL_Parameter >;
export using PixelPoint    = PosType    < PIXEL >;
export using PixelRectSize = SizeType   < PIXEL >;
export using PixelRect     = RectType   < PIXEL >;
export using PixelCircle   = CircleType < PIXEL >;
export using PixelEllipse  = EllipseType< PIXEL >;

export PixelPoint const PP_NULL(PixelPoint::NULL_VAL());   // compiler generates call!
export PixelPoint const PP_ZERO(PixelPoint::ZERO_VAL());   // compiler generates call!

export constexpr PIXEL operator"" _PIXEL(unsigned long long ull)
{
	return PIXEL(Cast2Short(ull));
}

/////////////////// fPixel ///////////////////////////////////

export using fPixel         = NamedType   <float, struct fPixel_Parameter>;
export using fPixelPoint    = PosType     <fPixel>;
export using fPixelLine     = LineType    <fPixel>;
export using fPixelRectSize = SizeType    <fPixel>;
export using fPixelRect     = RectType    <fPixel>;
export using fPixelCircle   = CircleType  <fPixel>;
export using fPixelEllipse  = EllipseType <fPixel>;

export fPixelPoint const fPP_NULL(fPixelPoint::NULL_VAL());   // compiler generates call!
export fPixelPoint const fPP_ZERO(fPixelPoint::ZERO_VAL());   // compiler generates call!

export constexpr fPixel operator"" _fPixel(long double ld)
{
	return fPixel(Cast2Float(ld));
}

/////////////////// conversions ///////////////////////////////////

export PIXEL Convert2PIXEL(float const f)
{
	return PIXEL(Cast2Long(f));
}

export PixelPoint operator*
(
	PixelPoint const& pp, 
	float      const  d
)
{ 
	PixelPoint ppRes
	{
		Convert2PIXEL(pp.GetXvalue() * d),
		Convert2PIXEL(pp.GetYvalue() * d)
	};
	return ppRes; 
}

export PixelPoint operator*(PixelPoint const& pp, long const d)
{ 
	return PixelPoint(PIXEL(pp.GetXvalue() * d), PIXEL(pp.GetYvalue() * d));
}

export PixelPoint operator/(PixelPoint const& pp, long const d)
{ 
	return PixelPoint(PIXEL(pp.GetXvalue() / d), PIXEL(pp.GetYvalue() / d));
}

export PIXEL Convert2PIXEL(fPixel const fPixel)
{
	return Convert2PIXEL(fPixel.GetValue());
}

export constexpr fPixel Convert2fPixel(PIXEL const pixel)
{
	return fPixel(Cast2Float(pixel.GetValue()));
}

export PixelPoint Convert2PixelPoint(fPixelPoint const fPixPoint)
{
	return PixelPoint(Convert2PIXEL(fPixPoint.GetX()), Convert2PIXEL(fPixPoint.GetY()));
}

export fPixelPoint Convert2fPixelPoint(PixelPoint const pixPoint)
{
	return fPixelPoint(Convert2fPixel(pixPoint.GetX()), Convert2fPixel(pixPoint.GetY()));
}

export PixelRectSize Convert2PixelRectSize(fPixelRectSize const fRectSize)
{
	return PixelRectSize(Convert2PIXEL(fRectSize.GetX()), Convert2PIXEL(fRectSize.GetY()));
}

export fPixelRectSize Convert2fPixelRectSize(PixelRectSize const rectSize)
{
	return fPixelRectSize(Convert2fPixel(rectSize.GetX()), Convert2fPixel(rectSize.GetY()));
}

export fPixelRect Convert2fPixelRect(PixelRect const & rect)
{
	return fPixelRect
	(
		Convert2fPixel(rect.GetLeft()),
		Convert2fPixel(rect.GetTop()),
		Convert2fPixel(rect.GetRight()+1),
		Convert2fPixel(rect.GetBottom()+1)
	);
}

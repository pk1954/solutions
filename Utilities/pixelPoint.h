// pixelPoint.h
//

#pragma once

#include <algorithm>   // max
#include <stdlib.h>    // abs
#include "util.h"     
#include "debug.h"
#include "pixel.h"

using PixelPoint = PointType< PIXEL >;

inline long const GetXvalue( PixelPoint const & pp ) { return pp.GetX().GetValue(); }
inline long const GetYvalue( PixelPoint const & pp ) { return pp.GetY().GetValue(); }

inline PixelPoint const operator+ (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator+ (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res += p; return res; };
inline PixelPoint const operator- (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res -= p; return res; };

inline PixelPoint const operator* (PixelPoint const a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const a, long const l) { PixelPoint res(a); res /= l; return res; };

inline PixelPoint const abs( PixelPoint const a ) { return PixelPoint( a.GetX().abs_value(), a.GetY().abs_value() ); }

inline PIXEL const MaxAbsDelta(PixelPoint const pp) 
{
    return PIXEL( max( abs( GetXvalue(pp) ), abs( GetYvalue(pp) ) ) );
}

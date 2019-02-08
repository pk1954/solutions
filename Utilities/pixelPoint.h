// pixelPoint.h
//

#pragma once

#include <algorithm>   // max
#include <stdlib.h>    // abs
#include "util.h"     
#include "debug.h"
#include "pixel.h"
#include "PointType.h"

using PixelPoint = PointType< PIXEL >;

inline PixelPoint const operator+ (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res += b; return res; };
inline PixelPoint const operator- (PixelPoint const a, PixelPoint const b) { PixelPoint res(a); res -= b; return res; };

inline PixelPoint const operator+ (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res += p; return res; };
inline PixelPoint const operator- (PixelPoint const a, PIXEL const p) { PixelPoint res(a); res -= p; return res; };

inline PixelPoint const operator* (PixelPoint const a, long const l) { PixelPoint res(a); res *= l; return res; };
inline PixelPoint const operator/ (PixelPoint const a, long const l) { PixelPoint res(a); res /= l; return res; };

inline PIXEL const MaxAbsDelta(PixelPoint const pp) 
{
    return PIXEL( max( abs( pp.GetXvalue() ), abs( pp.GetYvalue() ) ) );
}

// pixel.h
//

#pragma once

#include <limits.h>
#include <stdlib.h>    // abs
#include <sstream>
#include "NamedType.h"

using PIXEL = NamedType< long, struct PixelParameter >;

inline PIXEL const abs(PIXEL const a) { return PIXEL{ ::abs(a.GetValue()) }; }

inline PIXEL const operator+ (PIXEL const a, PIXEL const b) { PIXEL res(a); res += b; return res; }
inline PIXEL const operator- (PIXEL const a, PIXEL const b) { PIXEL res(a); res -= b; return res; }

inline PIXEL const operator* (PIXEL const a, long const l) { PIXEL res(a); res *= l; return res; }
inline PIXEL const operator/ (PIXEL const a, long const l) { PIXEL res(a); res /= l; return res; }

inline long const operator/ (PIXEL const a, PIXEL const b) { long res(a.GetValue()); res /= b.GetValue(); return res; }

PIXEL operator"" _PIXEL( unsigned long long );

std::wostream & operator << ( std::wostream &, PIXEL const );

PIXEL const PIXEL_NULL = PIXEL(LONG_MIN);

// pixel.h
//

#pragma once

#include <limits.h>
#include <sstream>
#include "NamedType.h"

using PIXEL = NamedType< long, struct PIXEL_PARAMETER, Comparable, Subtractable, Addable, Multiplicable, AbsValue >;

inline PIXEL const operator/ (PIXEL const a, long const l) { PIXEL res(a); res /= l; return res; }

inline long const operator/ (PIXEL const a, PIXEL const b) { long res(a.GetValue()); res /= b.GetValue(); return res; }

PIXEL operator"" _PIXEL( unsigned long long );

std::wostream & operator << ( std::wostream &, PIXEL const );

PIXEL const PIXEL_NULL = PIXEL(LONG_MIN);

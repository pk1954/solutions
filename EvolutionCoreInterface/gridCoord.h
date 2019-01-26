// gridCoord.h
//

#pragma once

#include <iostream>
#include "NamedType.h"

using GRID_COORD = NamedType< short, struct GRID_COORD_Parameter >;

inline bool IsEven( GRID_COORD const a ) { return a.GetValue() % 2 == 0; }
inline bool IsOdd ( GRID_COORD const a ) { return a.GetValue() % 2 != 0; }

inline GRID_COORD const abs(GRID_COORD const a) { return GRID_COORD( ::abs(a.GetValue()) ); }

inline GRID_COORD const operator+ (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res += b; return res; }
inline GRID_COORD const operator- (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res -= b; return res; }

constexpr GRID_COORD operator"" _GRID_COORD( unsigned long long );

std::wostream & operator << ( std::wostream &, GRID_COORD const );

GRID_COORD const MAX_GRID_COORD  = GRID_COORD(255_GRID_COORD);
GRID_COORD const GRID_COORD_NULL = GRID_COORD(SHRT_MIN);

static long const GRID_WIDTH_  = 200;
static long const GRID_HEIGHT_ = 100;
GRID_COORD const GRID_WIDTH  = GRID_COORD(200_GRID_COORD);
GRID_COORD const GRID_HEIGHT = GRID_COORD(100_GRID_COORD);
GRID_COORD const GRID_X_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_Y_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_X_MAX  = GRID_WIDTH  - GRID_COORD(1_GRID_COORD);
GRID_COORD const GRID_Y_MAX  = GRID_HEIGHT - GRID_COORD(1_GRID_COORD);

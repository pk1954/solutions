// gridCoord.h
//
// EvolutionCoreInterface

#pragma once

#include "util.h"
#include "NamedType.h"

using GRID_COORD = NamedType< short, struct GRID_Parameter >;

inline bool IsEven( GRID_COORD const a ) { return a.GetValue() % 2 == 0; }
inline bool IsOdd ( GRID_COORD const a ) { return a.GetValue() % 2 != 0; }

constexpr const GRID_COORD operator"" _GRID_COORD( const unsigned long long ull )
{
	return GRID_COORD( CastToShort( ull ) );
}

GRID_COORD const MAX_GRID_COORD{ 255_GRID_COORD };

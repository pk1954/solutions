// gridCoord.cpp
//
// library: EvolutionCore

#include "stdafx.h"
#include <iomanip>
#include "util.h"
#include "gridCoord.h"

using namespace std;

GRID_COORD operator"" _GRID_COORD( unsigned long long ull )
{
	return GRID_COORD( CastToUnsignedShort( ull ) );
}
 
// gridCoord.cpp
//
// library: EvolutionCore

#include "stdafx.h"
#include <iomanip>
#include "util.h"
#include "gridCoord.h"

using namespace std;

std::wostream & operator << ( std::wostream & out, GRID_COORD const c )
{
	out << setw( 5 ) << c.GetValue();
    return out;
}

GRID_COORD operator"" _GRID_COORD( unsigned long long ull )
{
	return GRID_COORD( CastToUnsignedShort( ull ) );
}
 
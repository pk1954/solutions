// pixel.cpp
//

#include "stdafx.h"
#include <iomanip>
#include "pixel.h"
#include "util.h"

PIXEL operator"" _PIXEL( unsigned long long ull )
{
	return PIXEL( CastToShort( ull ) );
}

wostream & operator << ( wostream & out, PIXEL const pixel )
{
    //lint -e747  Significant prototype coercion with setw
    out << setw( 5 ) << pixel.GetValue();
    return out;
}

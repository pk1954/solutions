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
    out << L" " << setw( 3 ) << pixel.GetValue();
	PIXEL p = PIXEL(1_PIXEL);
	p += PIXEL(3_PIXEL);
	PIXEL p2 = p + PIXEL(3_PIXEL);
    return out;
}

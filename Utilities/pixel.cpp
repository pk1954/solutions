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

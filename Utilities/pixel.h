// pixel.h
//

#pragma once

#include <limits.h>
#include <sstream>
#include "NamedType.h"

using namespace std;

using PIXEL = NamedType
< 
	long, 
	struct PIXEL_PARAMETER, 
	Comparable, 
	Addable, 
	Subtractable, 
	Multiplicable, 
	Dividable,
	AbsValue
>;

PIXEL operator"" _PIXEL( unsigned long long );

std::wostream & operator << ( std::wostream &, PIXEL const );

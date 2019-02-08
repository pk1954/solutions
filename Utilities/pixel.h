// pixel.h
//

#pragma once

#include <limits.h>
#include <sstream>
#include "NamedType.h"

using namespace std;

using PIXEL = NamedType< long >;

PIXEL operator"" _PIXEL( unsigned long long );

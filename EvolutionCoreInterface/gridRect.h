// gridRect.h : 
//

#pragma once

#include "gridPoint.h"
#include "RectType.h"

using GridRectSize = SizeType< GRID_COORD >;
using GridRect     = RectType< GRID_COORD >;

void Apply2Rect( GridPointFunc const &, GridRect const & );
void Apply2Rect( GridPointFunc const &, GridPoint const, GridPoint const, bool const = false );

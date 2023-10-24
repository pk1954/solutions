// GridRect.ixx : 
//
// EvolutionCoreInterface

export module GridRect;

import RectType;
import GridPoint;

using GridRectSize = SizeType< GRID_COORD >;
using GridRect = RectType< GRID_COORD >;

void Apply2Rect(GridPointFunc const&, GridPoint const, GridPoint const, bool const = false);

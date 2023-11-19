// GridRect.ixx : 
//
// EvolutionCore

export module GridRect;

import Types;
import GridPoint;
import GridCoord;

using GridRectSize = SizeType<GridCoord>;
using GridRect     = RectType<GridCoord>;

void Apply2Rect(GridPointFunc const&, GridPoint const, GridPoint const, bool const = false);

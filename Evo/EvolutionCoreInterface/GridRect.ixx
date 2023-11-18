// GridRect.ixx : 
//
// EvolutionCoreInterface

export module GridRect;

import RectType;
import GridPoint;

using GridRectSize = SizeType< GridCoord >;
using GridRect = RectType< GridCoord >;

void Apply2Rect(GridPointFunc const&, GridPoint const, GridPoint const, bool const = false);

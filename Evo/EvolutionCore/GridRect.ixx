// GridRect.ixx : 
//
// EvoCoreLib

export module EvoCoreLib:GridRect;

import Types;
import :GridPoint;
import :GridCoord;

export using GridRectSize = SizeType<GridCoord>;
export using GridRect     = RectType<GridCoord>;

export void Apply2Rect(GridPointFunc const&, GridPoint const, GridPoint const, bool const = false);

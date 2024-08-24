// GridCoord.ixx
//
// EvoCoreLib

export module EvoCoreLib:GridCoord;

import Types;
import SaveCast;

export using GridCoord = NamedType<short, struct GRID_Parameter>;

export inline bool IsEven(GridCoord const a) { return a.GetValue() % 2 == 0; }
export inline bool IsOdd (GridCoord const a) { return a.GetValue() % 2 != 0; }

export constexpr const GridCoord operator"" _GRID_COORD(const unsigned long long ull)
{
	return GridCoord(Cast2Short(ull));
}

export GridCoord const MAX_GRID_COORD{ 255_GRID_COORD };

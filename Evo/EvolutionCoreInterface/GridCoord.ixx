// GridCoord.ixx
//
// EvolutionCoreInterface

module;

import Types;

export module GridCoord;

export using GridCoord = NamedType< short, struct GRID_Parameter >;

export inline bool IsEven(GridCoord const a) { return a.GetValue() % 2 == 0; }
export inline bool IsOdd (GridCoord const a) { return a.GetValue() % 2 != 0; }

export constexpr const GridCoord operator"" _GridCoord(const unsigned long long ull)
{
	return GridCoord(CastToShort(ull));
}

export GridCoord const MAX_GRID_COORD{ 255_GridCoord };

// CoordPos.ixx
//
// BlokusCore

export module BlokusCore:CoordPos;

import std;
import Util;
import Types;
import SaveCast;

using std::numeric_limits;

export using Coord = NamedType<signed char, struct Coord_Parameter>;

export constexpr const Coord operator"" _COORD(const unsigned long long ull)
{
	return Coord(Cast2SignedChar(ull));
}

static Coord UndefinedCoord()
{
	static Coord constexpr res { (numeric_limits<signed char>::min)() };
	return res;
}

export int const MAX_SHAPE_EXTENSION { 5 };

const Coord MAX_COL { MAX_SHAPE_EXTENSION - 1};
const Coord MAX_ROW { MAX_SHAPE_EXTENSION - 1};

export using CoordPos = PosType<Coord>;

export inline CoordPos const UndefinedCoordPos { UndefinedCoord(), UndefinedCoord() };

bool IsInShapeRange(CoordPos const &pos)
{
    return IsInRange(pos.GetX(), 0_COORD, MAX_COL) && IsInRange(pos.GetY(), 0_COORD, MAX_ROW);
}

// BlokusCoords.ixx
//
// BlokusCore

export module BlokusCore:BlokusCoords;

import std;
import Util;
import Types;
import SaveCast;

using std::numeric_limits;
using std::round;

// integer coordinates

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

export using CoordRect = RectType<Coord>;

export bool IsInShapeRange(CoordPos const&);

// float coordinates

export extern MicroMeter const UM_CELL_SIZE;
export extern MicroMeter const UM_BOARD_SIZE;

export MicroMeter     Convert2fCoord(Coord     const);
export MicroMeterPnt  Convert2fCoord(CoordPos  const&);
export MicroMeterRect Convert2fCoord(CoordRect const&);

export Coord    Round2Coord   (MicroMeter const&);
export CoordPos Round2CoordPos(MicroMeterPnt const&);

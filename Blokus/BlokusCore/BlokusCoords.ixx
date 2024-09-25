// BlokusCoords.ixx
//
// BlokusCore

export module BlokusCore:BlokusCoords;

import std;
import Util;
import Types;
import SaveCast;

using std::numeric_limits;

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

bool IsInShapeRange(CoordPos const &pos)
{
    return IsInRange(pos.GetX(), 0_COORD, MAX_COL) && IsInRange(pos.GetY(), 0_COORD, MAX_ROW);
}

// floating coordinates

export using fCoord     = NamedType<float, struct fCoord_Parameter>;
export using fCoordPos  = PosType<fCoord>;
export using fCoordRect = RectType<fCoord>;

export constexpr const fCoord operator"" _fCOORD(const unsigned long double d)
{
	return fCoord(Cast2Float(d));
}

fCoord Convert2fCoord(Coord const coord) 
{ 
	return fCoord(static_cast<float>(coord.GetValue())); 
}

fCoordPos Convert2fCoord(CoordPos const& coordPos)
{
	return fCoordPos(Convert2fCoord(coordPos.GetX()), Convert2fCoord(coordPos.GetY()));
}

fCoordRect Convert2fCoord(CoordRect const& rect)
{
	return fCoordRect(Convert2fCoord(rect.GetStartPoint()), Convert2fCoord(rect.GetEndPoint()));
}
// Pos.ixx
//
// BlokusCore

export module BlokusCore:Pos;

import Util;
import Types;
import SaveCast;

export using Coord = NamedType<signed char, struct Coord_Parameter>;

export constexpr const Coord operator"" _COORD(const unsigned long long ull)
{
	return Coord(Cast2SignedChar(ull));
}

export int const MAX_SHAPE_EXTENSION { 5 };

const Coord MAX_COL { MAX_SHAPE_EXTENSION - 1};
const Coord MAX_ROW { MAX_SHAPE_EXTENSION - 1};

export struct Pos
{
    Coord m_x;
    Coord m_y;

	bool operator==(Pos const&) const = default;

    bool IsInShapeRange() const
    {
        return IsInRange(m_x, 0_COORD, MAX_COL) && IsInRange(m_y, 0_COORD, MAX_ROW);
    }

	fPixelPoint TofPixelPos(fPixel const size) const
	{
		return fPixelPoint(size * m_x.GetValue(), size * m_y.GetValue());
	}; 

};
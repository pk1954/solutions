// BoardMap.ixx
//
// BlokusCore

export module BlokusCore:BoardMap;

import std;
import Debug;
import :Components;

using std::array;

export class BoardMap
{
public:

    BoardMap()
    {
        Reset();
    }

    void Reset()
    {
    	for (int y = 0; y < BOARD_SIZE; ++y)
    	for (int x = 0; x < BOARD_SIZE; ++x)
	    	m_map[y][x] = true;
    }

    bool IsValidPos(CoordPos const& pos) const
    {
        return IsOnBoard(pos) && getCell(pos);
    }

    void SetCell(CoordPos const& pos,  bool const bVal)
    {
        if (IsOnBoard(pos))
            m_map[pos.GetYvalue()][pos.GetXvalue()] = bVal;
    }

private:

    bool getCell(CoordPos const& pos) const
    {
        return m_map[pos.GetYvalue()][pos.GetXvalue()];
    }

    array<array<bool, BOARD_SIZE>, BOARD_SIZE> m_map;
};
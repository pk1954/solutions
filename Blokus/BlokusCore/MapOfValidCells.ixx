// MapOfValidCells.ixx
//
// BlokusCore

export module BlokusCore:MapOfValidCells;

import std;
import Debug;
import :Components;

using std::array;
using std::endl;
using std::wcout;

export class MapOfValidCells  // occupied cells and cells which have piece of same color 
{                             // as neighbours, are false, otherwise true
public:

    MapOfValidCells()
    {
        Reset();
    }

    void Dump()
    {
        wcout << L"*** Map of valid cells ***" << endl;
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            for (int j = 0; j < BOARD_SIZE; ++j)
                wcout << m_map[i][j];
            wcout << endl;
        }
    }

    void Reset()
    {
    	for (int y = 0; y < BOARD_SIZE; ++y)
    	for (int x = 0; x < BOARD_SIZE; ++x)
	    	m_map[y][x] = true;
    }

    bool IsBlocked(CoordPos const& pos) const
    {
        return !(IsOnBoard(pos) && getCell(pos));
    }

    void BlockPosition(CoordPos const &pos)
    {
	    if (IsOnBoard(pos))
            m_map[pos.GetYvalue()][pos.GetXvalue()] = false;
    }

    void BlockNeighbours(CoordPos const &coordPos)
    {
        BlockPosition(NorthPos(coordPos));
        BlockPosition(EastPos (coordPos));
        BlockPosition(SouthPos(coordPos));
        BlockPosition(WestPos (coordPos));
    }

private:

    bool getCell(CoordPos const& pos) const
    {
        return m_map[pos.GetYvalue()][pos.GetXvalue()];
    }

    array<array<bool, BOARD_SIZE>, BOARD_SIZE> m_map;
};
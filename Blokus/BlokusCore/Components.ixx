// Components.ixx
//
// BlokusCore

export module BlokusCore:Components;

import std;
import :BlokusCoords;
import :PieceType;
import :PieceTypeId;

using std::array;

export inline int   const BOARD_SIZE       { 20 };
export inline Coord const COORD_BOARD_SIZE { BOARD_SIZE };
export inline Coord const MAX_COORD        { BOARD_SIZE - 1 };

export CoordPos NorthPos(CoordPos const& pos) { return CoordPos(pos.GetX(),           pos.GetY() - 1_COORD); }
export CoordPos EastPos (CoordPos const& pos) { return CoordPos(pos.GetX() + 1_COORD, pos.GetY()          ); }
export CoordPos SouthPos(CoordPos const& pos) { return CoordPos(pos.GetX(),           pos.GetY() + 1_COORD); }
export CoordPos WestPos (CoordPos const& pos) { return CoordPos(pos.GetX() - 1_COORD, pos.GetY()          ); }

export CoordPos NorthEastPos(CoordPos const& pos) { return CoordPos(pos.GetX() + 1_COORD, pos.GetY() - 1_COORD); }
export CoordPos SouthEastPos(CoordPos const& pos) { return CoordPos(pos.GetX() + 1_COORD, pos.GetY() + 1_COORD); }
export CoordPos NorthWestPos(CoordPos const& pos) { return CoordPos(pos.GetX() - 1_COORD, pos.GetY() - 1_COORD); }
export CoordPos SouthWestPos(CoordPos const& pos) { return CoordPos(pos.GetX() - 1_COORD, pos.GetY() + 1_COORD); }

export bool IsOnBoard(CoordPos const& pos)
{
    return IsInRange(pos.GetX(), 0_COORD, MAX_COORD) && 
           IsInRange(pos.GetY(), 0_COORD, MAX_COORD);
}

void Apply2AllBoardCells(auto const& func)
{
	for (Coord y = 0_COORD; y < COORD_BOARD_SIZE; ++y)
	for (Coord x = 0_COORD; x < COORD_BOARD_SIZE; ++x)
        func(CoordPos(x, y));
}

export class Components
{
public:

    static void Initialize();

    static void Apply2AllPieceTypes(auto const& func)
    {
        for (PieceType& pt : m_pieceTypes)
            func(pt);
    }

    static void Apply2AllPieceTypesC(auto const& func)
    {
        for (PieceType const& pt : m_pieceTypes)
            func(pt);
    }

    static PieceType const& GetPieceTypeC(PieceTypeId const id) 
    { 
        return m_pieceTypes.at(id.GetValue());
    }

private:

    inline static array<PieceType, NR_OF_PIECE_TYPES> m_pieceTypes;

    static void initPieceTypes();
};

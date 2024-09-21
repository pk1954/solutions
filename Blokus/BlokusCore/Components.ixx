// Components.ixx
//
// BlokusCore

export module BlokusCore:Components;

import std;
import :CoordPos;
import :PieceType;
import :PieceTypeId;

using std::array;

export inline int   const BOARD_SIZE       { 20 };
export inline Coord const COORD_BOARD_SIZE { BOARD_SIZE };
export inline Coord const MAX_COORD        { BOARD_SIZE - 1 };

export bool IsOnBoard(CoordPos const& pos)
{
    return IsInRange(pos.GetX(), 0_COORD, MAX_COORD) && 
            IsInRange(pos.GetY(), 0_COORD, MAX_COORD);
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

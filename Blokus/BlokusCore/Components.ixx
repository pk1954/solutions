// Components.ixx
//
// BlokusCore

export module BlokusCore:Components;

import std;
import :PieceType;
import :PieceTypeId;

using std::array;

export class Components
{
public:
    inline static int const BOARD_SIZE { 20 };

    static void Initialize();

    static void Apply2AllPieceTypes(auto const& func)
    {
        for (PieceType const& pt : m_pieceTypes)
            func(pt);
    }

    static PieceType const& GetPieceType(PieceTypeId const id) 
    { 
        return m_pieceTypes.at(id.GetValue());
    }

private:

    inline static array<PieceType, NR_OF_PIECE_TYPES> m_pieceTypes;

    static void initPieceTypes();
};
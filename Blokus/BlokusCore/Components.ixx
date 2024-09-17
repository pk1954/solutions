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
    inline static int BOARD_SIZE { 20 };

    static void Initialize();

    static void Apply2AllPieceTypes(auto const& func)
    {
        for (PieceType const& pt : m_pieceTypes)
            func(pt);
    }

private:

    inline static array<PieceType, NR_OF_PIECE_TYPES> m_pieceTypes;

    static void initPieceTypes();
};
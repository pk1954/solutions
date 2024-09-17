// Components.ixx
//
// BlokusCore

export module BlokusCore:Components;

import std;
import :PieceType;

using std::array;

export class Components
{
public:
    static void Initialize();

    static void Apply2AllPieceTypes(auto const& func)
    {
        for (PieceType const& pt : m_pieceTypes)
            func(pt);
    }

private:

    inline static array<PieceType, 21> m_pieceTypes;

    static void initPieceTypes();
};
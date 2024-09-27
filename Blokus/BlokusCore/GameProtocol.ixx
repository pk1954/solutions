// GameProtocol.ixx
//
// BlokusCore

export module BlokusCore:GameProtocol;

import std;
import :Move;
import :PlayerId;
import :PieceTypeId;

using std::array;

using ROUND = array<Move, NR_OF_PLAYERS>;

export class GameProtocol
{
public:
    void Initialize()
    {
        m_round = 0;
    }

    void Add(Move const& move)
    {
        int iPlayerIndex { move.GetPlayerId().GetValue() };
        m_list[m_round][iPlayerIndex-1] = move;
        if (iPlayerIndex == NR_OF_PLAYERS)
            ++m_round;
    }

private:
    int m_round { 0 };
    array<ROUND, NR_OF_PIECE_TYPES> m_list;
};
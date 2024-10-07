// MatchProtocol.ixx
//
// BlokusCore

export module BlokusCore:MatchProtocol;

import std;
import :Move;
import :PlayerId;
import :PieceTypeId;

using std::array;

using ROUND = array<Move, NR_OF_PLAYERS>;

export class MatchProtocol
{
public:
    void Reset()
    {
        m_round = 0;
    }

    void Add(Move const& move)
    {
        //PlayerId const id { move.GetPlayerId() };
        //m_list[m_round][id.GetValue()] = move;
        //if (i == LAST_PLAYER)
        //    ++m_round;
    }

private:
    int m_round { 0 };
    array<ROUND, NR_OF_PIECE_TYPES> m_list;
};
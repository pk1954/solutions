// MatchProtocol.ixx
//
// BlokusCore

export module BlokusCore:MatchProtocol;

import std;
import :BlokusMove;
import :PlayerId;
import :PieceTypeId;

using std::array;

struct PlayerMove
{
    PlayerMove() = default;

    PlayerMove(BlokusMove const &move) :
        m_coordPos   (move.GetCoordPos()),
        m_idShape    (move.GetShapeId()),
        m_idPieceType(move.GetPieceTypeId())
    {}

    CoordPos    m_coordPos;
    ShapeId     m_idShape;
    PieceTypeId m_idPieceType;
};

using ROUND = array<PlayerMove, NR_OF_PLAYERS>;

export class MatchProtocol
{
public:
    void Reset()
    {
        m_round = 0;
    }

    void Add(BlokusMove const& move)
    {
        PlayerId const id { move.GetPlayerId() };
        m_list[m_round][id.GetValue()] = move;
        if (id == LAST_PLAYER)
            ++m_round;
    }

private:
    int m_round { 0 };
    array<ROUND, NR_OF_PIECE_TYPES> m_list;
};
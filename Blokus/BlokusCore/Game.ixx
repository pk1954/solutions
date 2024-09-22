// Game.ixx
//
// BlokusCore

export module BlokusCore:Game;

import std;
import Types;
import Color;
import :Board;
import :Player;
import :Players;
import :PlayerId;
import :Components;

using std::vector;

export class Game
{
public:
    void Initialize()
    {
        m_players.Initialize();
    }

    void FindValidMoves(PlayerId const);

    Player& ActivePlayer()
    {
        return m_players.GetPlayer(m_activePlayer);
    }

    Player const& ActivePlayerC() const
    {
        return m_players.GetPlayerC(m_activePlayer);
    }

    void DrawSetPieces
    (
	    D2D_driver     const& d2d,
	    BlokusCoordSys const& coordSys
    ) const
    {
        Apply2AllBoardCells
        (
            [this, &d2d, &coordSys](CoordPos const& pos)
            {
		        PlayerId const idPlayer { m_board.GetPlayerId(pos) };
		        if (idPlayer != NO_PLAYER)
		        {
                    Player const& player { m_players.GetPlayerC(idPlayer) };
                    player.DrawCell(d2d, coordSys, pos);
		        }
            }
        );
    }

    void PerformMove(Move const& move)
    {
        ActivePlayer().PerformMove(move);
        m_board.PerformMove(move, m_players);
    }

    void NextPlayer()
    {
        if (++m_activePlayer == PlayerId(NR_OF_PLAYERS))
            m_activePlayer = PlayerId(0);
}

    bool NextMove()
    {
        FindValidMoves(m_activePlayer);
        if (m_validMoves.empty())
            return false;
        Move const &move { ActivePlayerC().SelectMove(m_validMoves) };
        PerformMove(move);
        return true;
    }

    void FindContactPnts(PlayerId const id)
    {
        Player &player { m_players.GetPlayer(id) };
        player.ClearContactPnts();
        Apply2AllBoardCells
        (
            [this, id, &player](CoordPos const& pos)
            {
                if (m_board.IsContactPnt(pos, id))
                    player.AddContactPnt(pos);
            }
        );
    }

private:
    Board        m_board;
    Players      m_players;
    PlayerId     m_activePlayer { 0 };
    vector<Move> m_validMoves;
};
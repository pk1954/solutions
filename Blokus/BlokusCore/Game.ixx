// Game.ixx
//
// BlokusCore

export module BlokusCore:Game;

import std;
import Types;
import Color;
import HiResTimer;
import :Board;
import :Player;
import :Players;
import :PlayerId;
import :Components;

using std::vector;

int g_iNrOfPieces;
int g_iNrOfShapes;
int g_iNrOfMoves;

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

    void DrawSetPieces(D2D_driver const&, BlokusCoordSys const&) const;
    void NextPlayer();
    bool NextMove();
    void FindContactPnts();

private:
    HiResTimer   m_timerFindContactPnts;
    HiResTimer   m_timerFindValidMoves;
    Board        m_board;
    Players      m_players;
    PlayerId     m_activePlayer { 0 };
    vector<Move> m_validMoves;
};
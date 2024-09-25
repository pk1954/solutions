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
    Game()
    {
        Initialize();
    }

    void Initialize()
    {
        m_board.Initialize();
        m_players.Initialize();
        m_bGameFinished = false;
        m_uiPlayersLeft = NR_OF_PLAYERS;
        m_activePlayer  = 0;
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

    bool GameFinished() { return m_bGameFinished; }

    void DrawSetPieces(BlokusDrawContext&) const;
    void NextPlayer();
    bool NextMove();
    void FindContactPnts();
    Player const& Winner();

private:
    bool         m_bGameFinished { false };
    unsigned int m_uiPlayersLeft { NR_OF_PLAYERS };
    HiResTimer   m_timerFindContactPnts;
    HiResTimer   m_timerFindValidMoves;
    Board        m_board;
    Players      m_players;
    PlayerId     m_activePlayer { 0 };
    vector<Move> m_validMoves;

    void playerFinished(Player&);
};
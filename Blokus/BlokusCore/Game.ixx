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
import :GameProtocol;

using std::vector;
using std::unique_ptr;

int g_iNrOfPieces;
int g_iNrOfShapes;
int g_iNrOfMoves;

class Game;

class RuleServer : public RuleServerInterface
{
public:
    RuleServer(Game &game)
        : m_game(game)
    {}

    vector<Move> const &GetListOfValidMoves() const final;
    Board        const &GetBoard()            const final;

private:
    Game &m_game;
};

export class Game
{
public:
    Game()
    {
        Initialize();
    }

    void Initialize();
    vector<Move> const& FindValidMoves();

    Player       &ActivePlayer ()       { return m_players.GetPlayer(m_activePlayer); }
    Player const &ActivePlayerC() const { return m_players.GetPlayerC(m_activePlayer); }

    bool          GameFinished() { return m_bGameFinished; }
    Player const &Winner()       { return m_players.GetPlayerC(WinnerId()); }
    PlayerId      WinnerId();

    void DrawSetPieces(DrawContext&) const;
    bool NextPlayer();
    bool NextMove();
    void FindContactPnts();

    Board const &GetBoard() const { return m_board; }

private:
    bool         m_bGameStarted  { false };
    bool         m_bGameFinished { false };
    unsigned int m_uiPlayersLeft { NR_OF_PLAYERS };
    HiResTimer   m_timerFindContactPnts;
    HiResTimer   m_timerFindValidMoves;
    Board        m_board;
    Players      m_players;
    PlayerId     m_activePlayer { 0 };
    vector<Move> m_validMoves;
    GameProtocol m_protocol;
    HiResTimer   m_timer;

    unique_ptr<RuleServer> m_upRuleServer; 

    bool isValidMove(Move const&, Player const&);
    void playerFinished(Player&);
};


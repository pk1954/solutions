// Match.ixx
//
// BlokusCore

export module BlokusCore:Match;

import std;
import Types;
import Color;
import HiResTimer;
import :Board;
import :Player;
import :Players;
import :PlayerId;
import :Components;
import :MatchProtocol;
import :RuleServerInterface;

using std::vector;
using std::unique_ptr;

int g_iNrOfPieces;
int g_iNrOfShapes;
int g_iNrOfMoves;

export class Match
{
public:
    Match();

    void Initialize();
    vector<Move> const& FindValidMoves();

    Player       &ActivePlayer ()       { return m_players.GetPlayer(m_activePlayer); }
    Player const &ActivePlayerC() const { return m_players.GetPlayerC(m_activePlayer); }

    Player const &GetPlayerC(PlayerId const id) const { return m_players.GetPlayerC(id); }
    Player       &GetPlayer (PlayerId const id)       { return m_players.GetPlayer(id); }

    bool          MatchFinished() { return m_bMatchFinished; }
    Player const &Winner()       { return m_players.GetPlayerC(WinnerId()); }
    PlayerId      WinnerId();

    void DrawSetPieces(DrawContext&) const;
    bool NextPlayer();
    bool NextMove();
    void FindContactPnts();

    Board const &GetBoard() const { return m_board; }

private:

    bool         m_bMatchStarted  { false };
    bool         m_bMatchFinished { false };
    unsigned int m_uiPlayersLeft { NR_OF_PLAYERS };
    Board        m_board;
    Players      m_players;
    PlayerId     m_activePlayer { 0 };
    vector<Move> m_validMoves;
    MatchProtocol m_protocol;
    HiResTimer   m_timerFindContactPnts;
    HiResTimer   m_timerFindValidMoves;
    HiResTimer   m_timer;

    unique_ptr<RuleServerInterface> m_upRuleServer; 

    bool isValidMove(Move const&, Player const&);
    void playerFinished(Player&);
};


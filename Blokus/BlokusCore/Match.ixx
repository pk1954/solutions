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

//int g_iNrOfPieces;
//int g_iNrOfShapes;
//int g_iNrOfMoves;
//
export class Match
{
public:
    Match();

    void Reset();
    void ResetTimers() { m_players.ResetTimers(); }

    vector<Move> const& FindValidMoves();

    Player       &ActivePlayer ()       { return m_players.GetPlayer(m_idActivePlayer); }
    Player const &ActivePlayerC() const { return m_players.GetPlayerC(m_idActivePlayer); }

    Player const &GetPlayerC(PlayerId const id) const { return m_players.GetPlayerC(id); }
    Player       &GetPlayer (PlayerId const id)       { return m_players.GetPlayer(id); }

    bool          PlayerHasFinished() { return ActivePlayer().HasFinished(); }
    bool          HasFinished()       { return m_uiPlayersLeft == 0; }
    Player const &Winner()            { return m_players.GetPlayerC(WinnerId()); }
    Board  const &GetBoard() const    { return m_board; }

    PlayerId WinnerId() const;

    void DrawSetPieces(DrawContext&) const;
    void NextMove();

private:

    unsigned int  m_uiPlayersLeft  { NR_OF_PLAYERS };
    Board         m_board;
    Players       m_players;
    PlayerId      m_idActivePlayer { 0 };
    vector<Move>  m_validMoves;
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
    //HiResTimer    m_timerFindValidMoves;
    HiResTimer    m_timer;
                  
    unique_ptr<RuleServerInterface> m_upRuleServer; 

    bool isValidMove (Move const&, Player const&);
    void testPosition(Move&, ShapeCoordPos const&);
    void testShape   (Move&, ShapeId       const);
    void testPiece   (Move&, Piece         const&);
    void findContactPnts();
};

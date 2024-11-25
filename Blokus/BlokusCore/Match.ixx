// Match.ixx
//
// BlokusCore

export module BlokusCore:Match;

import std;
import Types;
import Color;
import HiResTimer;
import :BlokusMove;
import :Board;
import :Components;
import :MatchProtocol;
import :Player;
import :PlayerId;
import :Players;
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

    void FindValidMoves(vector<BlokusMove>&);

    bool             PlayerHasFinished()                    { return ActivePlayer().HasFinished(); }
    bool             HasFinished()                          { return m_players.AllFinished(); }
    Player    const &Winner()                         const { return m_players.GetPlayerC(WinnerId()); }
    Board     const &GetBoard()                       const { return m_board; }
    PlayerId         ActivePlayerId()                       { return m_idActivePlayer; }
    Player          &ActivePlayer ()                        { return m_players.GetPlayer (m_idActivePlayer); }
    Player    const &ActivePlayerC()                  const { return m_players.GetPlayerC(m_idActivePlayer); }
    Player    const &GetPlayerC   (PlayerId const id) const { return m_players.GetPlayerC(id); }
    Player          &GetPlayer    (PlayerId const id)       { return m_players.GetPlayer(id); }
    PosDir          &GetPosDir    (BlokusMove const move)   { return GetPiece(move).GetPosDir(); }
    Piece     const &GetPieceC    (BlokusMove const)  const;
    PieceType const &GetPieceTypeC(BlokusMove const)  const;
    Piece           &GetPiece     (BlokusMove const);
    Degrees          GetRotation  (BlokusMove const)  const;
    BlokusMove       DoMove       ();
    void             UndoMove     (BlokusMove const);
    PlayerId         WinnerId()                       const;
    void             DrawSetPieces(DrawContext&)      const;
    PlayerId         NextPlayer();
    bool             IsValidPosition(BlokusMove const&) const;

private:

    Board         m_board;
    Players       m_players;
    PlayerId      m_idActivePlayer { 0 };
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
  //HiResTimer    m_timerFindValidMoves;
    HiResTimer    m_timer;
                  
    unique_ptr<RuleServerInterface> m_upRuleServer; 

    void testPosition(vector<BlokusMove>&, BlokusMove&, ShapeCoordPos const&);
    void testShape   (vector<BlokusMove>&, BlokusMove&, ShapeId       const);
    void testPiece   (vector<BlokusMove>&, BlokusMove&, Piece         const&);
    void findContactPnts();
};

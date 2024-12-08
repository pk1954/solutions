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
import :RuleServerInterface;
import :StrategyRandom;
import :StrategyTakeFirst;

using std::array;
using std::vector;
using std::unique_ptr;

StrategyTakeFirst StrategyRed;
StrategyTakeFirst StrategyGreen;
StrategyTakeFirst StrategyBlue;
StrategyRandom    StrategyYellow;

//int g_iNrOfPieces;
//int g_iNrOfShapes;
//int g_iNrOfMoves;
//
export class Match  : public RuleServerInterface
{
public:
    Match();

    void Reset();
    void ResetTimers();
    void GetListOfValidMoves(vector<BlokusMove>&) const;

    void Initialize()
    {
        m_players[0].Initialize(Components::GetPlayerType(PlayerId(0)), &StrategyRed);
        m_players[1].Initialize(Components::GetPlayerType(PlayerId(1)), &StrategyGreen); 
        m_players[2].Initialize(Components::GetPlayerType(PlayerId(2)), &StrategyBlue);
        m_players[3].Initialize(Components::GetPlayerType(PlayerId(3)), &StrategyYellow); 
    }

    Player    const &GetPlayerC(PlayerId const id)    const { return m_players.at(id.GetValue()); }
    Player          &GetPlayer (PlayerId const id)          { return m_players.at(id.GetValue()); }
    bool             HasFinished()                          { return IfAllPlayers([](Player const &p){ return p.HasFinished(); }); }
    Player    const &Winner()                         const { return GetPlayerC(WinnerId()); }
    Board     const &GetBoard()                       const { return m_board; }
    PlayerId         ActivePlayerId()                       { return m_idActivePlayer; }
    Player          &ActivePlayer ()                        { return GetPlayer (m_idActivePlayer); }
    Player    const &ActivePlayerC()                  const { return GetPlayerC(m_idActivePlayer); }
    PosDir          &GetPosDir    (BlokusMove const move)   { return GetPiece(move).GetPosDir(); }
    Piece     const &GetPieceC    (BlokusMove const)  const;
    PieceType const &GetPieceTypeC(BlokusMove const)  const;
    Color            ActiveColor  ()                 const { return ActivePlayerC().GetColor(); }
    Piece           &GetPiece     (BlokusMove const);
    Degrees          GetRotation  (BlokusMove const)  const;
    BlokusMove       DoMove       ();
    BlokusMove       DoMove       (BlokusMove);
    void             ResetPiece   (BlokusMove const move)  { GetPiece(move).Reset(); }
    void             UndoMove     (BlokusMove const);
    PlayerId         WinnerId()                                     const;
    void             DrawSetPieces (DrawContext&)                   const;
    void             DrawMovePiece (DrawContext&, BlokusMove const) const;
    PlayerId         NextPlayer();
    bool             IsNotBlocked       (BlokusMove const&) const;
    bool             IsValidPosition    (BlokusMove const&) const;
    bool             HasContact         (BlokusMove const&) const;

    void Apply2AllPlayersC(auto const& func) const
    {
        for (Player const& player: m_players)
            func(player);
    }

    void Apply2AllPlayers(auto const& func)
    {
        for (Player &player: m_players)
            func(player);
    }

    bool IfAllPlayers(auto const& func) const
    {
        for (Player const& player: m_players)
            if (!func(player))
                return false;
        return true;
    }

private:

    array<Player, NR_OF_PLAYERS> m_players;

    Board         m_board;
    PlayerId      m_idActivePlayer { 0 };
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
  //HiResTimer    m_timerFindValidMoves;
    HiResTimer    m_timer;
                  
    //unique_ptr<RuleServerInterface> m_upRuleServer; 

    void testPosition(vector<BlokusMove>&, BlokusMove&, ShapeCoordPos const&) const;
    void testShape   (vector<BlokusMove>&, BlokusMove&, ShapeId       const ) const;
    void testPiece   (vector<BlokusMove>&, BlokusMove&, Piece         const&) const;
    void findContactPnts();
};

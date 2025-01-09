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
import :Strategy;

using std::array;
using std::vector;
using std::unique_ptr;

export using PLAYERS = array<Player, NR_OF_PLAYERS>;

export class Match : public RuleServerInterface
{
public:
    static unique_ptr<Match> CreateNewMatch(Strategy * const, Strategy * const, Strategy * const, Strategy * const);

    Match(Strategy * const, Strategy * const, Strategy * const, Strategy * const);

    ListOfMoves const &GetListOfValidMoves(PlayerId const id) const final { return GetPlayerC(id).GetListOfValidMoves(); }
    Board       const &GetBoard()                             const final { return m_board; }

    void Dump() const;
    void Reset();
    void ResetTimers();
    void SetActivePlayer();
    void DoMove(BlokusMove);

    Player       &GetPlayer  (PlayerId   const id  )       { return m_players.at(id.GetValue()); }
    Player       &NextPlayer (Player     const &p  )       { return GetPlayer (::NextPlayer(p.GetPlayerId())); }
    Player const &NextPlayerC(Player     const &p  ) const { return GetPlayerC(::NextPlayer(p.GetPlayerId())); }
    Player const &PrevPlayerC(Player     const &p  ) const { return GetPlayerC(::PrevPlayer(p.GetPlayerId())); }
    Player const &GetPlayerC (PlayerId   const id  ) const { return m_players.at(id.GetValue()); }
    Player const &GetPlayerC (BlokusMove const move) const { return GetPlayerC(move.GetPlayerId()); }
    Player       &GetPlayer  (BlokusMove const move)       { return GetPlayer (move.GetPlayerId()); }
    Piece        &GetPiece   (BlokusMove const move)       { return GetPlayer(move).GetPiece(move.GetPieceTypeId()); }

    bool GameHasFinished() const { return IfAllPlayers([](Player const &p){ return p.HasFinished(); }); }

    bool       AnyShapeCellsBlocked(BlokusMove const) const;
    CoordPos   FindBestFit         (BlokusMove const) const;
    BlokusMove SelectMove          (PlayerId   const) const;
    PlayerId   WinnerId            ()                 const;


    void Apply2AllFreeCellsC(auto const& func) const
    {
    	m_board.Apply2AllFreeCellsC([&func](CoordPos const &p) { func(p); });
    }

   void Apply2AllPlayersC(auto const& func) const
    {
        for (Player const& player: m_players)
            func(player);
    }

    bool IfAllPlayers(auto const& func) const
    {
        for (Player const& player: m_players)
            if (!func(player))
                return false;
        return true;
    }

    PLAYERS       m_players;
    Board         m_board;
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
    HiResTimer    m_timer;
};

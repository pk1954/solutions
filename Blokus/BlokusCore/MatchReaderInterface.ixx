// MatchReaderInterface.ixx
//
// BlokusCore

export module BlokusCore:MatchReaderInterface;

import std;
import Types;
import Color;
import :Match;
import :BlokusMove;
import :Player;
import :PlayerId;
import :RuleServerInterface;

using std::vector;

export class MatchReaderInterface : public RuleServerInterface
{
public:

    Player      const &GetPlayerC(PlayerId const id)   const { return m_pMatch->m_players.at(id.GetValue()); }
    Player      const &Winner()                        const { return GetPlayerC(WinnerId()); }
    Board       const &GetBoard()                      const { return m_pMatch->m_board; }
    Piece       const &GetPieceC    (BlokusMove const) const;
    PieceType   const &GetPieceTypeC(BlokusMove const) const;
    Player      const &GetPlayerC   (BlokusMove const move)  const { return GetPlayerC(move.GetPlayerId()); }
                                                       
    ListOfMoves const &GetListOfValidMoves(PlayerId const id) const { return GetPlayerC(id).GetListOfValidMoves(); }

    void DrawSetPieces(DrawContext&)                                                      const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const)                       const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const, MicroMeterPnt const&) const;

    bool GameHasFinished() const { return IfAllPlayers([](Player const &p){ return p.HasFinished(); }); }

    BlokusMove SelectMove(Player const &player) const { return player.SelectMove(*this); }

    bool    IsValidPosition(BlokusMove const) const;
    bool    HasContact     (BlokusMove const) const;
    Degrees GetRotation    (BlokusMove const) const;

    PlayerId WinnerId() const;

    void Apply2AllFreeCellsC(auto const& func) const
    {
    	GetBoard().Apply2AllFreeCellsC([&func](CoordPos const &p) { func(p); });
    }

   void Apply2AllPlayersC(auto const& func) const
    {
        for (Player const& player: m_pMatch->m_players)
            func(player);
    }

    bool IfAllPlayers(auto const& func) const
    {
        for (Player const& player: m_pMatch->m_players)
            if (!func(player))
                return false;
        return true;
    }

protected:

    Match * m_pMatch;
};

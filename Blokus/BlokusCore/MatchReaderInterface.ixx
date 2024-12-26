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

    Color              ActiveColor()                   const { return ActivePlayerC().GetColor(); }
    Player      const &GetPlayerC(PlayerId const id)   const { return m_pMatch->m_players.at(id.GetValue()); }
    Player      const &Winner()                        const { return GetPlayerC(WinnerId()); }
    Board       const &GetBoard()                      const { return m_pMatch->m_board; }
    PlayerId          ActivePlayerId()                 const { return m_pMatch->m_idActivePlayer; }
    Player      const &ActivePlayerC()                 const { return m_pMatch->ActivePlayerC(); }
    ListOfMoves const &GetListOfValidMoves()           const { return ActivePlayerC().GetListOfValidMoves(); };
    Piece       const &GetPieceC    (BlokusMove const) const;
    PieceType   const &GetPieceTypeC(BlokusMove const) const;
                                                       
    void DrawSetPieces(DrawContext&)                                         const;
    void DrawMovePiece(DrawContext&, BlokusMove const)                       const;
    void DrawMovePiece(DrawContext&, BlokusMove const, MicroMeterPnt const&) const;

    bool HasFinished() const { return IfAllPlayers([](Player const &p){ return p.HasFinished(); }); }

    BlokusMove SelectMove() const { return ActivePlayerC().SelectMove(*this); }

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

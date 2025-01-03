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
    void Dump() const { m_pMatch->Dump(); }

    Player      const &Winner()                                   const { return GetPlayerC(WinnerId()); }
    Board       const &GetBoard()                                 const { return m_pMatch->m_board; }
    CoordPos           FindBestFit        (BlokusMove const move) const { return m_pMatch->FindBestFit(move); }
    Player      const &GetPlayerC         (BlokusMove const move) const { return m_pMatch->GetPlayerC(move); }
    bool              HasFinished         (BlokusMove const move) const { return GetPlayerC(move).HasFinished(); }
    Player      const &GetPlayerC         (PlayerId   const id)   const { return m_pMatch->m_players.at(id.GetValue()); }
    ListOfMoves const &GetListOfValidMoves(PlayerId   const id)   const { return GetPlayerC(id).GetListOfValidMoves(); }
    bool               HasFinished        (PlayerId   const id)   const { return GetPlayerC(id).HasFinished(); }
    Player      const &GetActivePlayer    ()                      const { return GetPlayerC(GetActivePlayerId()); }
    BlokusMove         SelectMove         ()                      const { return GetActivePlayer().SelectMove(*this); }

    void      Apply2AllFreeCellsC(auto const& func) const { m_pMatch->Apply2AllFreeCellsC(func); }
    void      Apply2AllPlayersC  (auto const& func) const { m_pMatch->Apply2AllPlayersC(func); }
    bool      IfAllPlayers       (auto const& func) const { return m_pMatch->IfAllPlayers(func); }
    bool      GameHasFinished    ()                 const { return m_pMatch->GameHasFinished(); }
    PlayerId  GetActivePlayerId  ()                 const { return m_pMatch->m_idPlayerActive; }

    void DrawSetPieces(DrawContext&)                                                      const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const)                       const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const, MicroMeterPnt const&) const;

    Piece      const &GetPieceC      (BlokusMove const) const;
    PieceType  const &GetPieceTypeC  (BlokusMove const) const;
    bool              IsValidPosition(BlokusMove const) const;
    bool              HasContact     (BlokusMove const) const;
    Degrees           GetRotation    (BlokusMove const) const;

    PlayerId WinnerId() const;

protected:

    Match * m_pMatch;
};

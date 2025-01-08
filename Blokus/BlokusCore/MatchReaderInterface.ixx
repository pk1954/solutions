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

using std::vector;

export class MatchReaderInterface
{
public:
    void Dump() const { m_pMatch->Dump(); }

    //ListOfMoves const &GetListOfValidMoves(PlayerId const id) const final { return GetPlayerC(id).GetListOfValidMoves(); }
    //Board       const &GetBoard()                             const final { return m_pMatch->m_board; }

    Player const &Winner             ()                      const { return GetPlayerC(WinnerId()); }
    PlayerId     WinnerId            ()                      const { return m_pMatch->WinnerId(); }
    CoordPos      FindBestFit        (BlokusMove const move) const { return m_pMatch->FindBestFit(move); }
    Player const &GetPlayerC         (BlokusMove const move) const { return m_pMatch->GetPlayerC(move); }
    Player const &GetPlayerC         (PlayerId   const id)   const { return m_pMatch->m_players.at(id.GetValue()); }
    Player const &NextPlayerC        (Player     const &p)   const { return m_pMatch->NextPlayerC(p); }
    Player const &PrevPlayerC        (Player     const &p)   const { return m_pMatch->PrevPlayerC(p); }
    BlokusMove    SelectMove         (Player     const &p)   const { return p.SelectMove(*m_pMatch); }
    bool          IfAllPlayers       (auto const& func)      const { return m_pMatch->IfAllPlayers(func); }
    bool          GameHasFinished    ()                      const { return m_pMatch->GameHasFinished(); }
    void          Apply2AllFreeCellsC(auto const& func)      const { m_pMatch->Apply2AllFreeCellsC(func); }
    void          Apply2AllPlayersC  (auto const& func)      const { m_pMatch->Apply2AllPlayersC(func); }

    void DrawSetPieces(DrawContext&)                                                      const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const)                       const;
    void DrawMovePiece(DrawContext&, BlokusMove const, Color const, MicroMeterPnt const&) const;

    Piece      const &GetPieceC      (BlokusMove const) const;
    PieceType  const &GetPieceTypeC  (BlokusMove const) const;
    bool              IsValidPosition(BlokusMove const) const;
    bool              HasContact     (BlokusMove const) const;
    Degrees           GetRotation    (BlokusMove const) const;

protected:

    Match * m_pMatch;
};

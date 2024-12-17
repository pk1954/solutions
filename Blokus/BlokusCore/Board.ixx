// Board.ixx
//
// BlokusCore

export module BlokusCore:Board;

import std;
import :Components;
import :BlokusUtilities;
import :PieceTypeId;
import :PlayerId;

using std::array;
using std::vector;

export class Board
{
public:
    Board();

    void Reset();
    void SetCell(CoordPos const&, PlayerId const, PieceTypeId const);
    void DoMove  (BlokusMove const&);
    void UndoMove(BlokusMove const&);

    PlayerId    GetPlayerId   (CoordPos const& pos) const { return getCellC(pos).idPlayer; }
    PieceTypeId GetPieceTypeId(CoordPos const& pos) const { return getCellC(pos).idPieceType; }

    bool     IsFreeCell    (CoordPos const&)                 const;
    bool     IsContactPnt  (CoordPos const&, PlayerId const) const;

private:
    bool hasPlayerId    (CoordPos const&, PlayerId const) const;
    bool hasDiagContact (CoordPos const&, PlayerId const) const;
    bool hasOrthoContact(CoordPos const&, PlayerId const) const;

    struct Cell
    {
        PlayerId    idPlayer;
        PieceTypeId idPieceType;
        void reset()
        {
            idPlayer    = NO_PLAYER;
            idPieceType = UndefinedPieceTypeId;
        }
    };

    array<array<Cell, BOARD_SIZE>, BOARD_SIZE> m_cells;

    Cell       &getCell (CoordPos const pos)       { return m_cells.at(pos.GetYvalue()).at(pos.GetXvalue()); }
    Cell const &getCellC(CoordPos const pos) const { return m_cells.at(pos.GetYvalue()).at(pos.GetXvalue()); }
};
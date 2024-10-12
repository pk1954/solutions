// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import :BlokusCoords;
import :PieceTypeId;
import :Components;
import :Move;

export class Piece
{
public:
    void Reset(PieceTypeId const id)
    {
        m_bAvailable  = true;
        m_idPieceType = id;
        SetPiecePos(GetPieceTypeC().GetInitialPos());
    }

    void SetPiecePos(CoordPos const& pos) { m_pos = pos; }
    void SetShapeId (ShapeId  const  id)  { m_idShape = id; }

    bool IsAvailable() const { return m_bAvailable; }

    CoordPos  const& GetPiecePos()    const { return m_pos; }
    PieceTypeId      GetPieceTypeId() const { return m_idPieceType; }
    PieceType const& GetPieceTypeC()  const { return Components::GetPieceTypeC(m_idPieceType); }

    void PerformMove(Move const& move)
    {
        m_bAvailable = false;
        SetPiecePos(move.GetCoordPos());
        SetShapeId (move.GetShapeId());
    }

private:
    CoordPos    m_pos         { -1_COORD, -1_COORD };
    ShapeId     m_idShape     { ShapeId(0) };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    bool        m_bAvailable  { true };
};
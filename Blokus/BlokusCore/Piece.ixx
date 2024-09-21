// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import :CoordPos;
import :PieceTypeId;
import :Components;
import :Move;

export class Piece
{
public:
    void Initialize(PieceTypeId const id)
    {
        m_idPieceType = id;
        m_pos = GetPieceTypeC().GetInitialPos();
    }

    void SetPos    (CoordPos const& pos) { m_pos = pos; }
    void SetShapeId(ShapeId  const  id)  { m_idShape = id; }

    bool IsAvailable() const { return m_pos.GetXvalue() > BOARD_SIZE;}

    CoordPos const&  GetPos()         const { return m_pos; }
    PieceTypeId      GetPieceTypeId() const { return m_idPieceType; }
    PieceType const& GetPieceTypeC()  const { return Components::GetPieceTypeC(m_idPieceType); }

    void PerformMove(Move const& move)
    {
        SetPos    (move.m_boardPos);
        SetShapeId(move.m_idShape);
    }

private:
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    CoordPos    m_pos         { -1_COORD, -1_COORD };
    ShapeId     m_idShape     { ShapeId(0) };
};
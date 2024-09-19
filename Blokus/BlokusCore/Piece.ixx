// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import :CoordPos;
import :PieceTypeId;
import :Components;

export class Piece
{
public:
    void Initialize(PieceTypeId const id)
    {
        m_id  = id;
        m_pos = GetPieceType().GetInitialPos();
    }

    bool IsAvailable() const { return m_pos.GetXvalue() < Components::BOARD_SIZE;}

    CoordPos const& GetPos()         const { return m_pos; }
    PieceTypeId     GetPieceTypeId() const { return m_id; }
    PieceType       GetPieceType()   const 
    {
        if (!IsValidPiecTypeId(m_id))
        {
            int x = 42;
        }
        return Components::GetPieceType(m_id); 
    }

private:
    PieceTypeId m_id  { UndefinedPieceTypeId };
    CoordPos    m_pos { -1_COORD, -1_COORD };
};
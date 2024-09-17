// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import :CoordPos;
import :PieceTypeId;
import :Components;

export class Piece
{
public:

    bool IsAvailable() { return m_pos.GetXvalue() < Components::BOARD_SIZE;}

private:
    PieceTypeId m_id;
    CoordPos    m_pos;
};
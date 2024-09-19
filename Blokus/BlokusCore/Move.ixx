// Move.ixx
//
// BlokusCore

export module BlokusCore:Move;

import :PieceTypeId;
import :CoordPos;

export struct Move
{
    PieceTypeId m_idPt;
    CoordPos    m_coordPos;
};
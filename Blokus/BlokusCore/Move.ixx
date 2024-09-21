// Move.ixx
//
// BlokusCore

export module BlokusCore:Move;

import :PlayerId;
import :PieceTypeId;
import :CoordPos;
import :ShapeId;
import :Shape;

export struct Move
{
    PlayerId    m_idPlayer    { UndefinedPlayerId()};
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    ShapeId     m_idShape     { UndefinedShapeId };
    CoordPos    m_boardPos    { UndefinedCoordPos };
};

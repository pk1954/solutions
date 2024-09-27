// Move.ixx
//
// BlokusCore

export module BlokusCore:Move;

import :PlayerId;
import :PieceTypeId;
import :BlokusCoords;
import :ShapeId;
import :Shape;

export class Move
{
public:

    PlayerId    GetPlayerId   () const { return m_idPlayer;    }
    PieceTypeId GetPieceTypeId() const { return m_idPieceType; }
    ShapeId     GetShapeId    () const { return m_idShape;     }
    CoordPos    GetCoordPos   () const { return m_boardPos;    }

    void SetPlayerId   (PlayerId    const idPlayer   ) { m_idPlayer    = idPlayer;    }
    void SetPieceTypeId(PieceTypeId const idPieceType) { m_idPieceType = idPieceType; }
    void SetShapeId    (ShapeId     const idShape    ) { m_idShape     = idShape;     }
    void SetCoordPos   (CoordPos    const coordPos   ) { m_boardPos    = coordPos;    }

    bool Undefined() { return m_idPlayer == NO_PLAYER; }

private:

    PlayerId    m_idPlayer    { NO_PLAYER };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    ShapeId     m_idShape     { UndefinedShapeId };
    CoordPos    m_boardPos    { UndefinedCoordPos };
};

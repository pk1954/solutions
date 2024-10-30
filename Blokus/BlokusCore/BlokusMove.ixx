// BlokusMove.ixx
//
// BlokusCore

export module BlokusCore:BlokusMove;

import :PlayerId;
import :PieceTypeId;
import :BlokusCoords;
import :ShapeId;

export class BlokusMove
{
public:
    //void Reset()
    //{
    //    m_idPlayer    = NO_PLAYER;
    //    m_idPieceType = UndefinedPieceTypeId;
    //    m_idShape     = UndefinedShapeId;
    //    m_boardPos    = UndefinedCoordPos;
    //}

    PlayerId    GetPlayerId   () const { return m_idPlayer;    }
    PieceTypeId GetPieceTypeId() const { return m_idPieceType; }
    ShapeId     GetShapeId    () const { return m_idShape;     }
    CoordPos    GetCoordPos   () const { return m_boardPos;    }

    void SetPlayerId   (PlayerId    const idPlayer   ) { m_idPlayer    = idPlayer;    }
    void SetPieceTypeId(PieceTypeId const idPieceType) { m_idPieceType = idPieceType; }
    void SetShapeId    (ShapeId     const idShape    ) { m_idShape     = idShape;     }
    void SetCoordPos   (CoordPos    const coordPos   ) { m_boardPos    = coordPos;    }

    bool Defined()   const { return m_idPlayer != NO_PLAYER; }
    bool Undefined() const { return m_idPlayer == NO_PLAYER; }

private:

    PlayerId    m_idPlayer    { NO_PLAYER };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    ShapeId     m_idShape     { UndefinedShapeId };
    CoordPos    m_boardPos    { UndefinedCoordPos };
};

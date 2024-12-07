// BlokusMove.ixx
//
// BlokusCore

export module BlokusCore:BlokusMove;

import :Components;
import :PlayerId;
import :PieceTypeId;
import :PieceType;
import :BlokusCoords;
import :ShapeId;
import :Shape;

export class BlokusMove
{
public:
    BlokusMove()
    {
        Reset();
    }

    BlokusMove
    (
        PlayerId    idPlayer,   
        PieceTypeId idPieceType,
        ShapeId     idShape,
        CoordPos    boardPos
    )
      : m_idPlayer   (idPlayer),   
        m_idPieceType(idPieceType),
        m_idShape    (idShape),
        m_boardPos   (boardPos)
    {}

    void Reset()
    {
        m_idPlayer    = NO_PLAYER;
        m_idPieceType = UndefinedPieceTypeId;
        m_idShape     = UndefinedShapeId;
        m_boardPos    = UndefinedCoordPos;
    }

    PlayerId    GetPlayerId   () const { return m_idPlayer;    }
    PieceTypeId GetPieceTypeId() const { return m_idPieceType; }
    ShapeId     GetShapeId    () const { return m_idShape;     }
    CoordPos    GetCoordPos   () const { return m_boardPos;    }

    PieceType const& GetPieceType ()       { return Components::GetPieceTypeC(GetPieceTypeId()); } 
    PieceType const& GetPieceTypeC() const { return Components::GetPieceType (GetPieceTypeId()); }
    Shape     const& GetShapeC    () const { return GetPieceTypeC().GetShapeC(GetShapeId()); }

    void SetPlayerId     (PlayerId    const idPlayer   ) { m_idPlayer    = idPlayer;    }
    void SetShapeId      (ShapeId     const idShape    ) { m_idShape     = idShape;     }
    void SetCoordPos     (CoordPos    const coordPos   ) { m_boardPos    = coordPos;    }
    void MoveCoordPos    (CoordPos    const coordPos   ) { m_boardPos   += coordPos;    }
    void SetPieceTypeId  (PieceTypeId const idPieceType) { m_idPieceType = idPieceType; }
    void ResetPieceTypeId()                              { m_idPieceType = UndefinedPieceTypeId; }
    void SetPieceType    (PieceType   const &pieceType ) { SetPieceTypeId(Components::GetPieceTypeId(pieceType)); }
    void SetInitialPos   ()                              { SetCoordPos(GetPieceTypeC().GetInitialPos()); }

    bool IsDefined()   const { return m_boardPos != UndefinedCoordPos; }
    bool IsUndefined() const { return m_boardPos == UndefinedCoordPos; }

private:

    PlayerId    m_idPlayer    { NO_PLAYER };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    ShapeId     m_idShape     { UndefinedShapeId };
    CoordPos    m_boardPos    { UndefinedCoordPos };
};

// BlokusMove.ixx
//
// BlokusCore

export module BlokusCore:BlokusMove;

import std;
import :Components;
import :PlayerId;
import :PieceTypeId;
import :PieceType;
import :BlokusCoords;
import :ShapeId;
import :Shape;

export using std::vector;

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
        CoordPos    coordPos
    )
      : m_idPlayer   (idPlayer),   
        m_idPieceType(idPieceType),
        m_idShape    (idShape),
        m_coordPos   (coordPos)
    {}

    void Reset()
    {
        m_idPlayer    = NO_PLAYER;
        m_idPieceType = UndefinedPieceTypeId;
        m_idShape     = UndefinedShapeId;
        m_coordPos    = UndefinedCoordPos;
    }

    PlayerId    GetPlayerId   () const { return m_idPlayer;    }
    PieceTypeId GetPieceTypeId() const { return m_idPieceType; }
    ShapeId     GetShapeId    () const { return m_idShape;     }
    CoordPos    GetCoordPos   () const { return m_coordPos;    }

    PieceType      & GetPieceType       ()       { return Components::GetPieceType (GetPieceTypeId()); } 
    PieceType const& GetPieceTypeC      () const { return Components::GetPieceType (GetPieceTypeId()); }
    Shape     const& GetShapeC          () const { return GetPieceTypeC().GetShapeC(GetShapeId()); }
    bool             IsCompletelyOnBoard() const { return GetShapeC().IsCompletelyOnBoard(m_coordPos); }

    void SetPlayerId     (PlayerId    const idPlayer   ) { m_idPlayer    = idPlayer;    }
    void SetShapeId      (ShapeId     const idShape    ) { m_idShape     = idShape;     }
    void SetCoordPos     (CoordPos    const coordPos   ) { m_coordPos    = coordPos;    }
    void MoveCoordPos    (CoordPos    const coordPos   ) { m_coordPos   += coordPos;    }
    void SetPieceTypeId  (PieceTypeId const idPieceType) { m_idPieceType = idPieceType; }
    void ResetPieceTypeId()                              { m_idPieceType = UndefinedPieceTypeId; }
    void SetPieceType    (PieceType   const &pieceType ) { SetPieceTypeId(Components::GetPieceTypeId(pieceType)); }
    void SetInitialPos   ()                              { SetCoordPos(GetPieceTypeC().GetInitialPos()); }

    bool IsDefined()   const { return m_coordPos != UndefinedCoordPos; }
    bool IsUndefined() const { return m_coordPos == UndefinedCoordPos; }

    void NextShape() 
    { 
        if ((++m_idShape).GetValue() >= GetPieceTypeC().NrOfShapes())
            m_idShape = ShapeId(0); 
    }

private:

    PlayerId    m_idPlayer    { NO_PLAYER };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
    ShapeId     m_idShape     { UndefinedShapeId };
    CoordPos    m_coordPos    { UndefinedCoordPos };
};

export using ListOfMoves = vector<BlokusMove>;

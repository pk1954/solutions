// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import Types;
import :BlokusCoords;
import :PieceTypeId;
import :Components;
import :BlokusMove;

export class Piece
{
public:
    void Reset(PieceTypeId const id)
    {
        m_bAvailable  = true;
        m_idPieceType = id;
        SetPiecePos(GetPieceTypeC().GetInitialPos());
    }

    void SetPiecePos(CoordPos const& pos) { m_posDir.m_umPos = Convert2fCoord(pos); }
    void SetShapeId (ShapeId  const  id)  { m_idShape = id; }

    bool IsAvailable() const { return m_bAvailable; }

	PosDir          &GetPosDir()            { return m_posDir; }
	PosDir    const &GetPosDirC()     const { return m_posDir; }
    PieceTypeId      GetPieceTypeId() const { return m_idPieceType; }
    PieceType const &GetPieceTypeC()  const { return Components::GetPieceTypeC(m_idPieceType); }

    void PerformMove(BlokusMove const& move)
    {
        m_bAvailable = false;
        SetPiecePos(move.GetCoordPos());
        SetShapeId (move.GetShapeId());
    }

private:
	PosDir      m_posDir;
    CoordPos    m_coordPos    { -1_COORD, -1_COORD };
    ShapeId     m_idShape     { ShapeId(0) };
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
	bool        m_bFlipped    { false };
    bool        m_bAvailable  { true };
};
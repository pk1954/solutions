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

    void SetPiecePos(CoordPos const& pos) { m_umPos = Convert2fCoord(pos); }
    void SetShapeId (ShapeId  const  id)  { m_idShape = id; }

    bool IsAvailable() const { return m_bAvailable; }

	MicroMeterPnt       &GetMicroMeterPos()        { return m_umPos; }
	MicroMeterPnt const &GetMicroMeterPosC() const { return m_umPos; }
    PieceTypeId          GetPieceTypeId()    const { return m_idPieceType; }
    PieceType     const  GetPieceTypeC()     const { return Components::GetPieceTypeC(m_idPieceType); }

    void PerformMove(BlokusMove const& move)
    {
        m_bAvailable = false;
        SetPiecePos(move.GetCoordPos());
        SetShapeId (move.GetShapeId());
    }

private:
	MicroMeterPnt m_umPos       { NP_NULL };
    CoordPos      m_coordPos    { -1_COORD, -1_COORD };
    ShapeId       m_idShape     { ShapeId(0) };
    PieceTypeId   m_idPieceType { UndefinedPieceTypeId };
	Degrees       m_rotation    { 0.0_Degrees };
	bool          m_bFlipped    { false };
    bool          m_bAvailable  { true };
};
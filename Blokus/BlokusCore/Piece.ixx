// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import Types;
import :BlokusCoords;
import :PieceTypeId;
import :Components;

export class Piece
{
public:
    void Initialize(PieceTypeId const id)
    {
        m_idPieceType = id;
        Reset();
    }

    void Reset()
    {
        m_bAvailable = true;
        setPiecePos(GetPieceTypeC().GetInitialPos());
        m_posDir.m_degrees = 0._Degrees;
    }

    bool IsAvailable() const { return m_bAvailable; }

	PosDir          &GetPosDir()            { return m_posDir; }
	PosDir    const &GetPosDirC()     const { return m_posDir; }
    PieceTypeId      GetPieceTypeId() const { return m_idPieceType; }
    PieceType const &GetPieceTypeC()  const { return Components::GetPieceTypeC(m_idPieceType); }

    void DoMove(CoordPos const& pos)
    {
        m_bAvailable = false;
        setPiecePos(pos);
    }

private:
	PosDir      m_posDir;
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };
	bool        m_bFlipped    { false };
    bool        m_bAvailable  { true };

    void setPiecePos(CoordPos const& pos) 
    { 
        m_posDir.m_umPos = Convert2fCoord(pos); 
    }
};
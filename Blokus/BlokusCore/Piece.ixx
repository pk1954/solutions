// Piece.ixx
//
// BlokusCore

export module BlokusCore:Piece;

import Debug;
import Types;
import Direct2D;
import :ShapeId;
import :BlokusMove;
import :BlokusCoords;
import :PieceTypeId;
import :Components;

export class Piece
{
public:
    void Initialize(PieceTypeId const);
    void Reset();
    void DoMove(BlokusMove const&);

    bool IsAvailable() const { return m_bAvailable; }

	PosDir          &GetPosDir()            { return m_posDir; }
	PosDir    const &GetPosDirC()     const { return m_posDir; }
    PieceTypeId      GetPieceTypeId() const { return m_idPieceType; }
    PieceType const &GetPieceTypeC()  const { return Components::GetPieceTypeC(m_idPieceType); }
    CoordPos         GetInitialPos()  const { return GetPieceTypeC().GetInitialPos(); }

	void Draw(DrawContext&, PosDir        const&, Color const, bool const)                const;
	void Draw(DrawContext&, MicroMeterPnt const&, Color const, bool const, ShapeId const) const;

    void StartMotion() { m_bAvailable = false; }

    void SetPos(MicroMeterPnt const &umPos)   { m_posDir.m_umPos = umPos; }
    void Move  (MicroMeterPnt const &umDelta) { m_posDir.m_umPos += umDelta; }

private:

    inline static TextFormatHandle m_hTextFormat { nullptr };

    // directly affected by move
    bool        m_bAvailable  { true };
    ShapeId     m_idShape     { 0 };
	PosDir      m_posDir;

    // not affected by move
    PieceTypeId m_idPieceType { UndefinedPieceTypeId };

    void setPiecePos(CoordPos const& pos) 
    { 
        m_posDir.m_umPos = Convert2fCoord(pos); 
    }
};
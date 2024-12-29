// PieceMotion.ixx
//
// BlokusCore

export module BlokusCore:PieceMotion;

import Types;
import DrawContext;
import :Player;
import :Piece;

export class PieceMotion
{
public:
    void Reset()
    {
        m_umPos  = NP_NULL;
        m_pPiece = nullptr;
    }

    bool FindPiece
    (
        Player        const &player,
        MicroMeterPnt const &umPos
    )
    {
	    m_pPiece = player.FindPiece
        (
            [&umPos, &player](Piece const &piece) 
            { 
                return IsInPiece(umPos, piece) && player.IsMoveable(piece.GetPieceTypeId()); 
            }
        );
	    if (m_pPiece != nullptr)
	    {
            m_umPos    = Convert2fCoord(m_pPiece->GetInitialPos());
	        m_umOffset = umPos - m_umPos;
            return true;
	    }
        return false;
    }

    bool MovePiece(MicroMeterPnt const& umPos)
    {
        CoordPos const coordPosOld { GetCoordPos() };
        m_umPos = umPos - m_umOffset;
        return GetCoordPos() != coordPosOld;
    }

    bool             IsActive     () const { return m_umPos != NP_NULL; }
    MicroMeterPnt    GetPosition  () const { return m_umPos; }
    CoordPos         GetCoordPos  () const { return Round2CoordPos(m_umPos); }
    Piece     const *GetPieceC    () const { return m_pPiece; }
    PieceType const &GetPieceTypeC() const { return m_pPiece->GetPieceTypeC(); }

private:

    Piece const * m_pPiece { nullptr };
    MicroMeterPnt m_umPos  { NP_NULL };
    MicroMeterPnt m_umOffset;
};
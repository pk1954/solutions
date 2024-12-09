// PieceMotion.ixx
//
// BlokusCore

export module BlokusCore:PieceMotion;

import Types;
import DrawContext;
import :Player;

export class PieceMotion
{
public:
    void Reset()
    {
        m_umPos = NP_NULL;
    }

    Piece const *SelectPiece
    (
        Player              &player,
        MicroMeterPnt const &umPos
    )
    {
	    Piece *pPiece { player.FindPiece([&umPos](Piece &piece) { return IsInPiece(umPos, piece); }) };
	    if (pPiece != nullptr)
	    {
            pPiece->StartMotion();
            m_umPos = Convert2fCoord(pPiece->GetPieceTypeC().GetInitialPos());
	        m_umOffset   = umPos - m_umPos;
	    }
        return pPiece;
    }

    bool MovePiece(MicroMeterPnt const& umPos)
    {
        CoordPos const coordPosOld { GetCoordPos() };
        m_umPos = umPos - m_umOffset;
        return GetCoordPos() != coordPosOld;
    }

    bool          IsActive   () const { return m_umPos != NP_NULL; }
    MicroMeterPnt GetPosition() const { return m_umPos; }
    CoordPos      GetCoordPos() const { return Round2CoordPos(m_umPos); }

private:

    MicroMeterPnt m_umPos { NP_NULL };
    MicroMeterPnt m_umOffset;
};
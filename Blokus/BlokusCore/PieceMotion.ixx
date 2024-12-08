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
        m_boardPos = UndefinedCoordPos;
    }

    Piece const *SelectPiece
    (
        Player             &player,
        MicroMeterPnt const &umPos
    )
    {
	    Piece *pPiece { player.FindPiece([&umPos](Piece &piece) { return IsInPiece(umPos, piece); }) };
	    if (pPiece != nullptr)
	    {
            pPiece->StartMotion();
		    m_boardPos = pPiece->GetPieceTypeC().GetInitialPos();
	        m_umOffset = umPos - Convert2fCoord(m_boardPos);
	    }
        return pPiece;
    }

    bool MovePiece(MicroMeterPnt const& umPos)
    {
        CoordPos      const boardPosOld { m_boardPos };
        MicroMeterPnt const umPosPiece  { umPos - m_umOffset };
        m_boardPos = Round2CoordPos(umPosPiece);
        return m_boardPos != boardPosOld;
    }

    bool     IsActive   () const { return m_boardPos != UndefinedCoordPos; }
    CoordPos GetPosition() const { return m_boardPos; }

private:

    CoordPos      m_boardPos { UndefinedCoordPos };
    MicroMeterPnt m_umOffset;
};
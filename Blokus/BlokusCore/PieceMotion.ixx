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
        Player        const &player,
        MicroMeterPnt const &umPos
    )
    {
	    Piece const *pPiece { player.FindPiece([&umPos](Piece const& piece) { return IsInPiece(umPos, piece); }) };
	    if (pPiece != nullptr)
	    {
		    PieceType const &pieceType { pPiece->GetPieceTypeC() };
		    m_boardPos = pieceType.GetInitialPos();
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
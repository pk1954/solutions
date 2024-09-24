// Player.ixx
//
// BlokusCore

export module BlokusCore:Player;

import std;
import Types;
import Color;
import Direct2D;
import :BlokusCoordSys;
import :BoardMap;
import :Piece;
import :PlayerId;
import :PieceTypeId;
import :Components;
import :CoordPos;
import :Move;

using std::array;
using std::vector;

export class Player
{
public:
    void Initialize(CoordPos const, Color const);

    bool IsFirstMove() const { return m_bFirstMove; }

    Piece const& GetPieceC(PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }
    Piece      & GetPiece (PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }

    void DrawFreePieces (D2D_driver const&, BlokusCoordSys const&) const;
    void DrawContactPnts(D2D_driver const&, BlokusCoordSys const&) const;
    void DrawCell       (D2D_driver const&, BlokusCoordSys const&, CoordPos const&) const;

    Move const& SelectMove(vector<Move> const&) const;
    void PerformMove(Move const&);

    void Apply2AllPieces(auto const& func)
    {
        for (Piece& piece: m_pieces)
            func(piece);
    }

    void Apply2AllPiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            func(piece);
    }

    void Apply2FreePiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            if (piece.IsAvailable())
                func(piece);
    }

    void Apply2AllContactPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_contactPntsOnBoard)
            func(pos);
    }

    void ClearContactPnts()
    {
        m_contactPntsOnBoard.clear();
    }

    void AddContactPnt(CoordPos const &pos)
    {
        m_contactPntsOnBoard.push_back(pos);
    }

    bool IsValidPos(CoordPos const &pos) const 
    { 
        return m_validPositions.IsValidPos(pos); 
    }

    void DoFinish()
    {
		m_bFinished = true;
		Apply2FreePiecesC
		(
			[this](Piece const& piece)
			{
				PieceType const& pt { piece.GetPieceTypeC() };
				m_bResult -= pt.NrOfCells();
			}
		);
    	if (m_remainingPieces == 0)
	    {
		    m_bResult = 15;
		    if (Components::GetPieceTypeC(m_pieceTypeIdMove).NrOfCells() == 1)
			    m_bResult += 5;
		    m_bFinished = true;
	    }
    }

    bool HasFinished() const { return m_bFinished; }
    int  Result()      const { return m_bResult; }

private:
   // static Random m_random;

    PieceTypeId                     m_pieceTypeIdMove { UndefinedPieceTypeId };
    unsigned int                    m_remainingPieces { NR_OF_PIECE_TYPES };
    int                             m_bResult         { 0 };
    bool                            m_bFinished       { false };
    bool                            m_bFirstMove      { true };
    Color                           m_color;
    array<Piece, NR_OF_PIECE_TYPES> m_pieces;
    vector<CoordPos>                m_contactPntsOnBoard;
    BoardMap                        m_validPositions;

    fPixelPoint getCenter(BlokusCoordSys const&, CoordPos const&) const;
    void reduceValidMoves(Move const&, PieceType const&);
};
// Player.ixx
//
// BlokusCore

export module BlokusCore:Player;

import std;
import Types;
import Color;
import Direct2D;
import :Piece;
import :PieceTypeId;
import :CoordPos;
import :Move;

using std::array;
using std::vector;

export class Player
{
public:
    Player
    (
        CoordPos const startPoint,
        Color    const col
    )
      : m_color(col)
    {
        m_contactPoints.push_back(startPoint);
    }

    void InitPieces()
    {
        PieceTypeId id { 0 };
        Apply2AllPieces
        (
           [&id](Piece& p){ p.Initialize(id++); }
        );
    }

    void DrawPieces(D2D_driver const&, fPixelPoint const, fPixel const) const;

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

    void FindValidMoves()
    {
        m_validMoves.clear();
        Apply2AllPiecesC
        (
            [this](Piece const& piece)
            {
                if (piece.IsAvailable())
		            piece.GetPieceType().Apply2AllOrientationsC
		            (
			            [](Shape const& shape)
			            {
				            shape.Apply2AllContactPntsC
				            (
					            [](CoordPos const & pos) 
                                { 
                                 //   func(pos); 
                                }
				            );
			            }
		            );
            }
        );
    }

private:
    Color m_color;

    array<Piece, NR_OF_PIECE_TYPES> m_pieces;

    vector<CoordPos> m_contactPoints;
    vector<Move>     m_validMoves;

};
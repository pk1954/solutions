// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import Types;
import Color;
import Direct2D;
import :Components;

void Player::DrawPieces
(
	D2D_driver  const &d2d, 
	fPixelPoint const  fPixPntOriginPieces,
	fPixel      const  fPixFieldSize
) const
{
	Apply2AllPiecesC
	(
		[this, &d2d, &fPixPntOriginPieces, &fPixFieldSize](Piece const& piece)
		{
			fPixelPoint const fPos 
			{ 
				fPixFieldSize * piece.GetPos().GetXvalue() + fPixFieldSize * 0.5f,
				fPixFieldSize * piece.GetPos().GetYvalue() + fPixFieldSize * 0.5f
			};
			piece.GetPieceType().Draw
			(
				d2d,
				fPixPntOriginPieces + fPos,
				m_color,
				fPixFieldSize
			);
		}
	);
}

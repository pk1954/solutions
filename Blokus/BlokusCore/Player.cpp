// Player.cpp
//
// BlokusCore

module BlokusCore:Player;

import Types;
import Color;
import Direct2D;
import :Components;
import :CoordPos;
import :Util;

fPixelPoint Player::getCenter
(
	BlokusCoordSys const& coordSys,
	CoordPos       const& pos
) const
{
	fPixel      const fPixCellSize { coordSys.CellSize() };
	fPixel      const fPixHalfSize { fPixCellSize * 0.5f };
	fPixelPoint const fPos         { coordSys.Transform2fPixelPos(pos) + fPixelPoint(fPixHalfSize) };
	return fPos;
}

void Player::DrawFreePieces
(
	D2D_driver     const& d2d,
	BlokusCoordSys const& coordSys
) const
{
	Apply2FreePiecesC
	(
		[this, &d2d, &coordSys](Piece const& piece)
		{
			piece.GetPieceTypeC().Draw
			(
				d2d,
				getCenter(coordSys, piece.GetPos()),
				m_color,
				coordSys.CellSize()
			);
		}
	);
}

void Player::DrawCell
(
	D2D_driver     const& d2d,
	BlokusCoordSys const& coordSys,
	CoordPos       const& pos
) const
{
	ShapeSquare(d2d, getCenter(coordSys, pos), m_color, coordSys.CellSize() );
}

void Player::DrawContactPnts
(
	D2D_driver     const& d2d,
	BlokusCoordSys const& coordSys
) const
{
	Apply2AllContactPntsC
	(
		[this, &d2d, &coordSys](CoordPos const& pos)
		{
		    Color const COL_CONTACT_POINT { Color(0.5f, 0.5f, 0.5f) };
			ColSquare(d2d, getCenter(coordSys, pos), COL_CONTACT_POINT, coordSys.CellSize() );
		}
	);
}

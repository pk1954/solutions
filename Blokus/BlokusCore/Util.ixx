// Util.ixx
//
// BlokusCore

export module BlokusCore:Util;

import std;
import Types;
import Color;
import Direct2D;
import :BlokusCoordSys;

void colSquare
(
	D2D_driver  const &d2d,
	fPixelPoint const  center,
	Color       const  col,
	fPixel      const  size
)
{
	fPixel         const fPixHalfSize  { size * 0.5f };
	fPixelPoint    const fPixPntOrigin { center - fPixelPoint(fPixHalfSize, fPixHalfSize) };
	fPixelRectSize const fPixSize      { fPixelRectSize(size, size) };
	d2d.FillRectangle(fPixelRect(fPixPntOrigin, fPixSize), col);
}

export void ShapeSquare
(
    D2D_driver  const &d2d,
	BlokusCoordSys    &coordSys,
	CoordPos    const &pos,
	Color       const  col
)
{
	fPixelPoint const center { coordSys.GetCenter(pos) };
	fPixel      const size   { coordSys.CellSize() };
	colSquare(d2d, center,  col,        size       );
	colSquare(d2d, center,  col * 0.6f, size * 0.8f);
}

export void SmallDot
(
    D2D_driver  const &d2d,
	BlokusCoordSys    &coordSys,
	CoordPos    const &pos,
	Color       const  col
)
{
	fPixelPoint const center { coordSys.GetCenter(pos) };
	fPixel      const size   { coordSys.CellSize() };
	d2d.FillCircle(fPixelCircle(center, size * 0.2f), col);
}


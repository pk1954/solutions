// Util.ixx
//
// BlokusCore

export module BlokusCore:Util;

import std;
import Types;
import Color;
import Direct2D;

export void ColSquare
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
	fPixelPoint const  center,
	Color       const  col,
	fPixel      const  size
)
{
	ColSquare(d2d, center,  col,        size       );
	ColSquare(d2d, center,  col * 0.6f, size * 0.8f);
}


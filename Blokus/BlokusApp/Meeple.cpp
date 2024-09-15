// Meeple.cpp
//
// Blokus

module Meeple;

import std;

using std::array;

void Meeple::colSquare
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

void Meeple::meepleSquare
(
    D2D_driver  const &d2d,
	fPixelPoint const  center,
	Color       const  col,
	fPixel      const  size
)
{
	colSquare(d2d, center,  col,        size       );
	colSquare(d2d, center,  col * 0.6f, size * 0.8f);
}

void Meeple::Draw
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
)
{
	fPixelPoint center { pos };
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			if (m_shape[y][x])
				meepleSquare(d2d, center, col, size);
			center.Move2Horz(size);
		}
		center.SetX(pos.GetX());
		center.Move2Vert(size);
	}
}

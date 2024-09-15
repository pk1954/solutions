// Shape.cpp
//
// BlokusCore

module Shape;

import std;
import Types;
import Color;
import Direct2D;

using std::swap;

void Shape::colSquare
(
	D2D_driver  const &d2d,
	fPixelPoint const  center,
	Color       const  col,
	fPixel      const  size
) const
{
	fPixel         const fPixHalfSize  { size * 0.5f };
	fPixelPoint    const fPixPntOrigin { center - fPixelPoint(fPixHalfSize, fPixHalfSize) };
	fPixelRectSize const fPixSize      { fPixelRectSize(size, size) };
	d2d.FillRectangle(fPixelRect(fPixPntOrigin, fPixSize), col);
}

void Shape::shapeSquare
(
    D2D_driver  const &d2d,
	fPixelPoint const  center,
	Color       const  col,
	fPixel      const  size
) const
{
	colSquare(d2d, center,  col,        size       );
	colSquare(d2d, center,  col * 0.6f, size * 0.8f);
}

bool Shape::spaceAtTop() const
{
	for (int x = 0; x < COLS; ++x)
		if (m_shape[0][x])
			return false;
	return true;
}

bool Shape::spaceAtLeft() const
{
	for (int y = 0; y < ROWS; ++y)
		if (m_shape[y][0])
			return false;
	return true;
}

void Shape::shiftTop()
{
	for (int x = 0; x < COLS; ++x)
	{
		for (int y = 1; y < ROWS; ++y)
			m_shape[y-1][x] = m_shape[y][x];
		m_shape[ROWS-1][x] = false;
	}
}

void Shape::shiftLeft()
{
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 1; x < COLS; ++x)
			m_shape[y][x-1] = m_shape[y][x];
		m_shape[y][COLS-1] = false;
	}
}

void Shape::normalize()
{
	while(spaceAtTop())
		shiftTop();
	while(spaceAtLeft())
		shiftLeft();
}

void Shape::Flip()
{
	for (int y = 1; y < ROWS; ++y)
		for (int x = 0; x < y; ++x)
			swap(m_shape[y][x], m_shape[x][y]);
	normalize();
}

void Shape::Rotate()
{
	SHAPE rotShape;
	for (int y = 0; y < ROWS; ++y)
		for (int x = 0; x < COLS; ++x)
			rotShape[y][x] = m_shape[(COLS-1)-x][y];
	m_shape = rotShape;
	normalize();
}

void Shape::Draw
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
) const
{
	fPixelPoint center { pos };
	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLS; ++x)
		{
			if (m_shape[y][x])
				shapeSquare(d2d, center, col, size);
			center.Move2Horz(size);
		}
		center.SetX(pos.GetX());
		center.Move2Vert(size);
	}
}

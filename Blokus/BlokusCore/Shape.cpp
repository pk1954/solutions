// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import std;
import Types;
import Color;
import Direct2D;

using std::swap;

// A Pos in the environment of a Shape is a contact Pos
// if at least one diagonal neighbour of the Pos is part of the Shape
// and no orthogonal (horz or vert) neighbour of the Pos is part of the Shape

bool Shape::diagContact(Pos const& pos) const
{
	return IsPartOfShape(Pos(pos.m_x + 1, pos.m_y + 1)) ||
		   IsPartOfShape(Pos(pos.m_x + 1, pos.m_y - 1)) ||
		   IsPartOfShape(Pos(pos.m_x - 1, pos.m_y + 1)) ||
		   IsPartOfShape(Pos(pos.m_x - 1, pos.m_y - 1));
}

bool Shape::orthoContact(Pos const& pos) const
{
	if (IsPartOfShape(Pos(pos.m_x,     pos.m_y + 1)))
		return true;
	if (IsPartOfShape(Pos(pos.m_x,     pos.m_y - 1)))
		return true;
	if (IsPartOfShape(Pos(pos.m_x + 1, pos.m_y    )))
		return true;
	if (IsPartOfShape(Pos(pos.m_x - 1, pos.m_y    )))
		return true;
	return false;
  //return IsPartOfShape(Pos(pos.m_x,     pos.m_y + 1)) ||
	//	   IsPartOfShape(Pos(pos.m_x,     pos.m_y - 1)) ||
	//	   IsPartOfShape(Pos(pos.m_x + 1, pos.m_y    )) ||
	//	   IsPartOfShape(Pos(pos.m_x - 1, pos.m_y    ));
}

Shape::Shape(SHAPE const &shape)
	: m_shape(shape)
{
	Pos candidate;
	for (candidate.m_x = -1_COORD; candidate.m_x <= 5_COORD; ++candidate.m_x)
	for (candidate.m_y = -1_COORD; candidate.m_y <= 5_COORD; ++candidate.m_y)
		if (!IsPartOfShape(candidate) && diagContact(candidate) && !orthoContact(candidate))
			m_contactPnts.push_back(candidate);
}

bool Shape::spaceAtTop() const
{
	for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
		if (m_shape[0][x])
			return false;
	return true;
}

bool Shape::spaceAtLeft() const
{
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
		if (m_shape[y][0])
			return false;
	return true;
}

void Shape::shiftTop()
{
	for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
	{
		for (int y = 1; y < MAX_SHAPE_EXTENSION; ++y)
			m_shape[y-1][x] = m_shape[y][x];
		m_shape[MAX_SHAPE_EXTENSION-1][x] = false;
	}
}

void Shape::shiftLeft()
{
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
	{
		for (int x = 1; x < MAX_SHAPE_EXTENSION; ++x)
			m_shape[y][x-1] = m_shape[y][x];
		m_shape[y][MAX_SHAPE_EXTENSION-1] = false;
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
	for (int y = 1; y < MAX_SHAPE_EXTENSION; ++y)
		for (int x = 0; x < y; ++x)
			swap(m_shape[y][x], m_shape[x][y]);
	normalize();
}

void Shape::Rotate()
{
	SHAPE rotShape;
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
		for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
			rotShape[y][x] = m_shape[(MAX_SHAPE_EXTENSION-1)-x][y];
	m_shape = rotShape;
	normalize();
}

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

void Shape::drawShapeSquares
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
) const
{
	fPixelPoint center { pos };
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
	{
		for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
		{
			if (m_shape[y][x])
				shapeSquare(d2d, center, col, size);
			center.Move2Horz(size);
		}
		center.SetX(pos.GetX());
		center.Move2Vert(size);
	}
}

void Shape::drawContactPoints
(
	D2D_driver  const &d2d,
	fPixelPoint const  fPixPosShape, 
	fPixel      const  size
) const
{
	Apply2AllContactPnts
	(
		[this, &d2d, fPixPosShape, size](Pos const& posContactPnt)
		{
			Color const CONTACT_POINT_COLOR { Color(0.5f, 0.5f, 0.5f) };
			fPixelPoint const pos 
			{
				fPixPosShape + posContactPnt.TofPixelPos(size)
			}; 
			colSquare(d2d, pos, CONTACT_POINT_COLOR, size);
		}
	);
}

void Shape::Draw
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
) const
{
	drawShapeSquares (d2d, pos, col, size);
//	drawContactPoints(d2d, pos, size);
}

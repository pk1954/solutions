// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import std;
import Types;
import Color;
import Direct2D;

using std::swap;

// A CoordPos in the environment of a Shape is a contact CoordPos
// if at least one diagonal neighbour of the CoordPos is part of the Shape
// and no orthogonal (horz or vert) neighbour of the CoordPos is part of the Shape

bool Shape::diagContact(CoordPos const& pos) const
{
	return IsPartOfShape(CoordPos(pos.GetX() + 1_COORD, pos.GetY() + 1_COORD)) ||
		   IsPartOfShape(CoordPos(pos.GetX() + 1_COORD, pos.GetY() - 1_COORD)) ||
		   IsPartOfShape(CoordPos(pos.GetX() - 1_COORD, pos.GetY() + 1_COORD)) ||
		   IsPartOfShape(CoordPos(pos.GetX() - 1_COORD, pos.GetY() - 1_COORD));
}

bool Shape::orthoContact(CoordPos const& pos) const
{
    return IsPartOfShape(CoordPos(pos.GetX(),           pos.GetY() + 1_COORD)) ||
		   IsPartOfShape(CoordPos(pos.GetX(),           pos.GetY() - 1_COORD)) ||
		   IsPartOfShape(CoordPos(pos.GetX() + 1_COORD, pos.GetY()          )) ||
		   IsPartOfShape(CoordPos(pos.GetX() - 1_COORD, pos.GetY()          ));
}

Shape::Shape(SHAPE const &shape)
	: m_shape(shape)
{
	for (int x = -1; x <= 5; ++x)
	for (int y = -1; y <= 5; ++y)
	{
		CoordPos candidate { Coord(x), Coord(y) };
		if (!IsPartOfShape(candidate) && diagContact(candidate) && !orthoContact(candidate))
			m_contactPnts.push_back(candidate);

	}
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
	fPixelPoint const  fPixPntShapePos, 
	Color       const  col,
	fPixel      const  size
) const
{
	Apply2AllShapeCells
	(
		[this, &d2d, &fPixPntShapePos, &col, size](ShapeCoordPos const& pos)
		{
			fPixelPoint center 
			{ 
				fPixPntShapePos.GetX() + size * Cast2Float(pos.GetXvalue()), 
				fPixPntShapePos.GetY() + size * Cast2Float(pos.GetYvalue())
			};
			shapeSquare(d2d, center, col, size);
		}
	);
}

void Shape::drawContactPoints
(
	D2D_driver  const &d2d,
	fPixelPoint const  fPixPosShape, 
	fPixel      const  size
) const
{
	Apply2AllContactPntsC
	(
		[this, &d2d, fPixPosShape, size](ShapeCoordPos const& posContactPnt)
		{
			static Color const CONTACT_POINT_COLOR { Color(0.5f, 0.5f, 0.5f) };
			colSquare
			(
				d2d, 
				fPixPosShape + TofPixelPos(posContactPnt, size), 
				CONTACT_POINT_COLOR, 
				size
			);
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

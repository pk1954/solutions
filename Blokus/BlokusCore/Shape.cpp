// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import std;
import Types;
import Color;
import Direct2D;
import :Util;

using std::swap;

bool Shape::isPartOfShape(CoordPos const& pos) const
{
	return IsInShapeRange(pos) && m_shape[pos.GetYvalue()][pos.GetXvalue()];
}

bool Shape::isCornerPnt(CoordPos const& pos) const
{
	return (!isPartOfShape(northPos(pos)) && !isPartOfShape(eastPos (pos))) ||
		   (!isPartOfShape(eastPos (pos)) && !isPartOfShape(southPos(pos))) ||
		   (!isPartOfShape(southPos(pos)) && !isPartOfShape(westPos (pos))) ||
		   (!isPartOfShape(westPos (pos)) && !isPartOfShape(northPos(pos)));
}

// A CoordPos in the environment of a Shape is a contact CoordPos
// if at least one diagonal neighbour of the CoordPos is part of the Shape
// and no orthogonal (horz or vert) neighbour of the CoordPos is part of the Shape

bool Shape::diagContact(CoordPos const& pos) const
{
	return isPartOfShape(northEastPos(pos)) || isPartOfShape(southEastPos(pos)) ||
		   isPartOfShape(northWestPos(pos)) || isPartOfShape(southWestPos(pos));
}

bool Shape::orthoContact(CoordPos const& pos) const
{
    return isPartOfShape(northPos(pos)) || isPartOfShape(eastPos (pos)) ||
		   isPartOfShape(southPos(pos)) || isPartOfShape(westPos (pos));
}

bool Shape::isContactPnt(CoordPos const& pos) const
{
	return !isPartOfShape(pos) && 
		    diagContact  (pos) && 
		   !orthoContact (pos);
}

Shape::Shape(SHAPE const &shape)
	: m_shape(shape)
{
	ShapeCoordPos pos;
	for (pos.SetY(-1_COORD); pos.GetY() <= MAX_ROW + 1_COORD; pos.IncY())
	for (pos.SetX(-1_COORD); pos.GetX() <= MAX_ROW + 1_COORD; pos.IncX())
		if (isContactPnt(pos))
			m_contactPnts.push_back(pos);

	Apply2AllShapeCells
	(
		[this](CoordPos const& pos)
		{
			if (isCornerPnt(pos))
			   m_cornerPnts.push_back(pos);
		}
	);
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
			ShapeSquare(d2d, center, col, size);
		}
	);
	//Apply2AllCornerPntsC
	//(
	//	[this, &d2d, &fPixPntShapePos, &col, size](ShapeCoordPos const& pos)
	//	{
	//		fPixelPoint center 
	//		{ 
	//			fPixPntShapePos.GetX() + size * Cast2Float(pos.GetXvalue()), 
	//			fPixPntShapePos.GetY() + size * Cast2Float(pos.GetYvalue())
	//		};
	//		d2d.FillCircle(fPixelCircle(center, size *0.2f));
	//	}
	//);
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
			ColSquare
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

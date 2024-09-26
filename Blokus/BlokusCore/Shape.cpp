// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import Color;
import :BlokusPreferences;
import :BlokusUtilities;
import :Components;

using std::swap;

bool Shape::isPartOfShape(ShapeCoordPos const& pos) const
{
	return IsInShapeRange(pos) && m_shape[pos.GetYvalue()][pos.GetXvalue()];
}

bool Shape::isCornerPnt(CoordPos const& pos) const
{
	return (!isPartOfShape(NorthPos(pos)) && !isPartOfShape(EastPos (pos))) ||
		   (!isPartOfShape(EastPos (pos)) && !isPartOfShape(SouthPos(pos))) ||
		   (!isPartOfShape(SouthPos(pos)) && !isPartOfShape(WestPos (pos))) ||
		   (!isPartOfShape(WestPos (pos)) && !isPartOfShape(NorthPos(pos)));
}

void Shape::CollectCornerPnts()
{
	Apply2AllShapeCellsC
	(
		[this](CoordPos const& pos)
		{
			if (isCornerPnt(pos))
			   m_cornerPnts.push_back(pos);
		}
	);
}

Shape::Shape(SHAPE const &shape)
	: m_shape(shape)
{}

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

void Shape::Draw
(
	DrawContext &context,
	Color       const  col
) const
{
	Apply2AllShapeCellsC
	(
		[this, &context, &col](ShapeCoordPos const& shapePos)
		{
			ShapeSquare(context, shapePos, col);
		}
	);
	if (BlokusPreferences::m_bShowCornerCells.Get())
	{
		Apply2AllCornerPntsC
		(
			[this, &context](ShapeCoordPos const& shapePos)
			{
				SmallDot(context, shapePos, Color(0.0f, 0.0f, 0.0f));
			}
		);
	}
}

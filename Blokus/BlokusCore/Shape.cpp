// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import Color;
import :BlokusPreferences;
import :BlokusUtilities;
import :Components;

using std::swap;

bool Shape::IsCompletelyOnBoard(CoordPos const& pos) const
{
	return IsTrueForAllCornerPnts
	(
		[&pos](ShapeCoordPos const& posCorner)
		{ 
			return IsOnBoard(pos + posCorner);
		}
	);
}

bool Shape::isPartOfShape(ShapeCoordPos const& pos) const
{
	return IsInShapeRange(pos) && m_shapeCells[pos.GetYvalue()][pos.GetXvalue()];
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
	m_cornerPnts.clear();
	Apply2AllShapeCellsC
	(
		[this](CoordPos const& pos)
		{
			if (isCornerPnt(pos))
			   m_cornerPnts.push_back(pos);
		}
	);
}

int Shape::CountCells() const
{
	int iCount = 0;
	Apply2AllShapeCellsC([&iCount](ShapeCoordPos const&){ ++iCount; });
	return iCount;
}

Shape::Shape(ShapeCells const &shapeCells)
	: m_shapeCells(shapeCells)
{}

bool Shape::spaceAtTop() const
{
	for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
		if (m_shapeCells[0][x])
			return false;
	return true;
}

bool Shape::spaceAtLeft() const
{
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
		if (m_shapeCells[y][0])
			return false;
	return true;
}

void Shape::shiftTop()
{
	for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
	{
		for (int y = 1; y < MAX_SHAPE_EXTENSION; ++y)
			m_shapeCells[y-1][x] = m_shapeCells[y][x];
		m_shapeCells[MAX_SHAPE_EXTENSION-1][x] = false;
	}
}

void Shape::shiftLeft()
{
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
	{
		for (int x = 1; x < MAX_SHAPE_EXTENSION; ++x)
			m_shapeCells[y][x-1] = m_shapeCells[y][x];
		m_shapeCells[y][MAX_SHAPE_EXTENSION-1] = false;
	}
}

void Shape::normalize()
{
	while(spaceAtTop())
		shiftTop();
	while(spaceAtLeft())
		shiftLeft();
	m_umPntCenter = CenterOfGravity();
	if (m_degRotation == 270._Degrees)
		m_degRotation = -90._Degrees;
}

void Shape::Flip()
{
	for (int y = 1; y < MAX_SHAPE_EXTENSION; ++y)
		for (int x = 0; x < y; ++x)
			swap(m_shapeCells[y][x], m_shapeCells[x][y]);
	normalize();
}

void Shape::Rotate()
{
	ShapeCells rotShapeCells;
	for (int y = 0; y < MAX_SHAPE_EXTENSION; ++y)
		for (int x = 0; x < MAX_SHAPE_EXTENSION; ++x)
			rotShapeCells[y][x] = m_shapeCells[(MAX_SHAPE_EXTENSION-1)-x][y];
	m_shapeCells = rotShapeCells;
	m_degRotation += 90._Degrees;
	normalize();
}

MicroMeterPnt Shape::CenterOfGravity()
{
	MicroMeterPnt umPntCenter { NP_ZERO };
	Apply2AllShapeCellsC
	(
		[&umPntCenter](ShapeCoordPos const &pos)
		{
			umPntCenter += Convert2fCoord(pos);
		}
	);
	umPntCenter /= Cast2Float(m_shapeCells.size());
	return umPntCenter;
}

void Shape::Draw
(
	DrawContext &context, 
	Color const col,
	bool  const bHighlighted
) const
{
	Apply2AllShapeCellsC
	(
		[this, &context, &col, bHighlighted](ShapeCoordPos const& shapePos)
		{
			ShapeSquare(context, shapePos, col, bHighlighted);
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

void Shape::Draw
(
	DrawContext  &context,
	Degrees const degRotation,
	Color   const col,
	bool    const bHighlighted
) const
{
	if (degRotation == 0._Degrees)
		Draw(context, col, bHighlighted);
	else
	{
		context.Push();
		context.SetRotation(degRotation, m_umPntCenter);
		Draw(context, col, bHighlighted);
		context.Pop();
	}
}

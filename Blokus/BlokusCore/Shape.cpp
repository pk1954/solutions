// Shape.cpp
//
// BlokusCore

module BlokusCore:Shape;

import Color;
import :BlokusPreferences;
import :BlokusUtilities;
import :Components;

using std::swap;
using std::wstring;
using std::to_wstring;

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

Shape::Shape
(
	ShapeCells  const &shapeCells, 
	PieceTypeId const  id
)
	: m_shapeCells(shapeCells),
	  m_idPieceType(id)
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

void Shape::colSquare
(
	DrawContext   const &context, 
	MicroMeterPnt const  center,
	Color         const  col,
	MicroMeter    const  umHalfSize
) const
{
	MicroMeterPnt const umPntStartPos { center - MicroMeterPnt(umHalfSize) };
	MicroMeterPnt const umPntEndPos   { center + MicroMeterPnt(umHalfSize) };
	context.FillRectangle(MicroMeterRect(umPntStartPos, umPntEndPos), col);
}

void Shape::shapeSquare
(
	DrawContext      const &context, 
	CoordPos         const &coordPos,
	Color            const  col,
	bool             const  bHighlighted,
	wstring          const &text,
	TextFormatHandle const  hTextFormat
) const
{
	MicroMeterPnt  const umPos       { Convert2fCoord(coordPos) };
	MicroMeter     const umHalfSize  { UM_CELL_SIZE * 0.5f };
	MicroMeterPnt  const umPosCenter { umPos + MicroMeterPnt(umHalfSize) };
	colSquare(context, umPosCenter, bHighlighted ? brighter(col) : col,         umHalfSize       );
	colSquare(context, umPosCenter, bHighlighted ? col           : darker(col), umHalfSize * 0.8f);
	if (BlokusPreferences::m_bShowPieceNumbers.Get())
	{
		MicroMeterRect umRect { umPos, UM_CELL_SIZE };
		context.DisplayText(umRect.Move2Vert(umHalfSize), text, hTextFormat);
	}
}

void Shape::Draw
(
	DrawContext           &context, 
	Color            const col,
	bool             const bHighlighted,
	TextFormatHandle const hTextFormat
) const
{
	wstring const text { to_wstring(m_idPieceType.GetValue()) };
	Apply2AllShapeCellsC
	(
		[this, &context, &col, bHighlighted, &text, hTextFormat](ShapeCoordPos const& shapePos)
		{
			shapeSquare(context, shapePos, col, bHighlighted, text, hTextFormat);
		}
	);
	if (BlokusPreferences::m_bShowCornerCells.Get())
	{
		Apply2AllCornerPntsC
		(
			[this, &context](ShapeCoordPos const& shapePos)
			{
				SmallDot(context, shapePos, COL_BLACK);
			}
		);
	}
}

void Shape::Draw
(
	DrawContext           &context,
	Degrees          const degRotation,
	Color            const col,
	bool             const bHighlighted,
	TextFormatHandle const hTextFormat
) const
{
	if (degRotation == 0._Degrees)
		Draw(context, col, bHighlighted, hTextFormat);
	else
	{
		context.Push();
		context.SetRotation(degRotation, m_umPntCenter);
		Draw(context, col, bHighlighted, hTextFormat);
		context.Pop();
	}
}

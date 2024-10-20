// PieceType.cpp
//
// BlokusCore

module BlokusCore:PieceType;

import DrawContext;

using std::array;

void PieceType::SetPos(int const x, int const y) 
{ 
	m_initialPos.SetX(Coord(Cast2SignedChar(x))); 
	m_initialPos.SetY(Coord(Cast2SignedChar(y))); 
}

void PieceType::addIfNew(Shape const &shape)
{
	for (Shape const& s : m_shapes)
		if (shape == s)
			return;
	m_shapes.push_back(shape);
}

void PieceType::addOrientations(Shape &shape)
{
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
}

void PieceType::SetShape(const SHAPE& shape)
{
	Shape shapeNew(shape);
	m_iNrOfCells = shapeNew.CountCells();
	m_shapes.clear();
	addOrientations(shapeNew);
	shapeNew.Flip();
	addOrientations(shapeNew);
	for (Shape &shape : m_shapes)
		shape.CollectCornerPnts();
}

void PieceType::Draw
(
	DrawContext         &context,
	MicroMeterPnt const &pos, 
	Color         const  col
) const
{
	fPixelPoint const offsetSave { context.GetPixelOffset() };
	context.Move(pos);
	m_shapes.at(0).Draw(context, col);
	context.SetPixelOffset(offsetSave);
}

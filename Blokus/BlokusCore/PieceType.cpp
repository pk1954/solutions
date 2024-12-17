// PieceType.cpp
//
// BlokusCore

module BlokusCore:PieceType;

import DrawContext;

using std::array;
using std::wstring;

void PieceType::SetPos(int const x, int const y) 
{ 
	m_initialPos.SetX(Coord(Cast2SignedChar(x))); 
	m_initialPos.SetY(Coord(Cast2SignedChar(y))); 
}

void PieceType::addIfNew(Shape const &shape)
{
	for (Shape const& s : m_shapes)
		if (shape.Equals(s))
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

void PieceType::SetShape(const ShapeCells& shapeCells)
{
	Shape shapeNew(shapeCells, m_idPieceType);
	m_iNrOfCells = shapeNew.CountCells();
	m_shapes.clear();
	addOrientations(shapeNew);
	shapeNew.Flip();
	addOrientations(shapeNew);
	for (Shape &shape : m_shapes)
		shape.CollectCornerPnts();
}

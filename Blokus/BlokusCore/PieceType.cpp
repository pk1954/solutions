// PieceType.cpp
//
// BlokusCore

module BlokusCore:PieceType;

using std::array;

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

void PieceType::initialize(SHAPE const &shape)
{
	Shape shapeNew(shape);
	addOrientations(shapeNew);
	shapeNew.Flip();
	addOrientations(shapeNew);
	for (Shape &shape : m_shapes)
		shape.CollectCornerPnts();
}

PieceType& PieceType::operator=(const SHAPE& shape)
{
	initialize(shape);
	return *this;
}

void PieceType::Draw
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
) const
{
	m_shapes.at(0).Draw(d2d, pos, col, size);
}

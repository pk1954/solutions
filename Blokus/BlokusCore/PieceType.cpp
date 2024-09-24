// PieceType.cpp
//
// BlokusCore

module BlokusCore:PieceType;

import :Util;

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
	shapeNew.Apply2AllShapeCellsC
	(
		[this](ShapeCoordPos const&){++m_iNrOfCells;}
	);
}

PieceType& PieceType::operator=(const SHAPE& shape)
{
	initialize(shape);
	return *this;
}

void PieceType::Draw
(
	D2D_driver const &d2d,
	BlokusCoordSys   &coordSys,
	CoordPos   const &pos, 
	Color      const  col
) const
{
	fPixelPoint const offsetSave = coordSys.Add2Offset(coordSys.Transform2fPixelSize(pos));
	m_shapes.at(0).Draw(d2d, coordSys, col);
	coordSys.SetOffset(offsetSave);
}

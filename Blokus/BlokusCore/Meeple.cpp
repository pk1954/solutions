// Meeple.cpp
//
// BlokusCore

module Meeple;

import std;

using std::array;

void Meeple::addIfNew(Shape const &shape)
{
	for (Shape const& s : m_orientations)
		if (shape == s)
			return;
	m_orientations.push_back(shape);
}

void Meeple::addOrientations(Shape &shape)
{
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
	shape.Rotate();
	addIfNew(shape);
}

void Meeple::initialize(SHAPE const &shape)
{
	Shape shapeNew(shape);
	addOrientations(shapeNew);
	shapeNew.Flip();
	addOrientations(shapeNew);
}

Meeple::Meeple()
{
}

Meeple::Meeple(SHAPE const &shape)
{
	initialize(shape);
}

Meeple& Meeple::operator=(const SHAPE& shape)
{
	initialize(shape);
	return *this;
}

void Meeple::Draw
(
	D2D_driver  const &d2d,
	fPixelPoint const  pos, 
	Color       const  col,
	fPixel      const  size
) const
{
	m_orientations.at(0).Draw(d2d, pos, col, size);
}

// Shape.ixx
//
// BlokusCore

export module Shape;

import std;
import Types;
import Color;
import Direct2D;

using std::array;

const int COLS { 5 };
const int ROWS { 5 };

export using SHAPE = array<array<bool,COLS>,ROWS>;

export class Shape
{
public:
	Shape() = default;

	Shape(SHAPE const &shape)
		: m_shape(shape)
	{}

	bool operator==(const Shape&) const = default;

	void Flip();
	void Rotate();

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

private:

    SHAPE m_shape;

	void colSquare  (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void shapeSquare(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	bool spaceAtTop() const;
	bool spaceAtLeft() const;
	void shiftTop();
	void shiftLeft();
	void normalize();
};
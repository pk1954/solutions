// Meeple.ixx
//
// Blokus

export module Meeple;

import std;
import Types;
import Color;
import Direct2D;

using std::array;

const int COLS { 5 };
const int ROWS { 3 };

export using SHAPE = array<array<bool,COLS>,ROWS>;

export class Meeple
{
public:

	Meeple(SHAPE const &shape)
		: m_shape(shape)
	{}

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const);

private:

    SHAPE m_shape;

	void colSquare   (D2D_driver const &, fPixelPoint const, Color const, fPixel const);
	void meepleSquare(D2D_driver const &, fPixelPoint const, Color const, fPixel const);
};
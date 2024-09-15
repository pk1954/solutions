// Meeple.ixx
//
// BlokusCore

export module Meeple;

import std;
import Types;
import Color;
import Direct2D;
import Shape;

using std::vector;

export class Meeple
{
public:

	Meeple();
	Meeple(SHAPE const &);
	Meeple& operator=(const SHAPE&);

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

	Shape const& StdOrientation() const { return m_orientations.at(0); }

	void Apply2AllOrientations(auto const& func) const
	{
		for (Shape const& s : m_orientations)
			func(s);
	}

	void Flip()
	{
		//ActiveOrientation().Flip();
	}

	void Rotate()
	{
		//ActiveOrientation().Rotate();
	}

private:
	void initialize(SHAPE const&);
	void addIfNew(Shape const&);
	void addOrientations(Shape&);

    vector<Shape> m_orientations;
};
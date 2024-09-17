// PieceType.ixx
//
// BlokusCore

export module BlokusCore:PieceType;

import std;
import Debug;
import SaveCast;
import Types;
import Color;
import Direct2D;
import :Shape;
import :CoordPos;

using std::vector;

export class PieceType
{
public:

	PieceType& operator=(const SHAPE&);

	void SetPos(int const x, int const y) 
	{ 
		m_position.SetX(Coord(Cast2SignedChar(x))); 
		m_position.SetY(Coord(Cast2SignedChar(y))); 
	}

	CoordPos GetPos() const { return m_position; }

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
	void addIfNew  (Shape const&);
	void addOrientations(Shape&);

    vector<Shape> m_orientations;
	CoordPos      m_position;
};
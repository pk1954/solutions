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
		m_initialPos.SetX(Coord(Cast2SignedChar(x))); 
		m_initialPos.SetY(Coord(Cast2SignedChar(y))); 
	}

	CoordPos GetInitialPos() const { return m_initialPos; }

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

	Shape const& StdOrientation() const { return m_orientations.at(0); }

	void Apply2AllOrientationsC(auto const& func) const
	{
		for (Shape const& s : m_orientations)
			func(s);
	}

    void Apply2AllContactPntsC(auto const& func) const
    {
		Apply2AllOrientationsC
		(
			[&func](Shape const& s)
			{
				s.Apply2AllContactPntsC
				(
					[&func](CoordPos const & pos) { func(pos); }
				);
			}
		);
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
	CoordPos      m_initialPos;
};
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
import :ShapeId;
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

	void SetPos(CoordPos const& newPos) { m_initialPos = newPos; }
	CoordPos GetInitialPos() const { return m_initialPos; }

	Shape const& GetShapeC(ShapeId const id) const { return m_shapes.at(id.GetValue()); }

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

	Shape const& StdOrientation() const { return m_shapes.at(0); }

	void Apply2AllShapeIdsC(auto const& func) const
	{
		for (int i = 0; i < m_shapes.size(); ++i)
			func(ShapeId(i));
	}

	void Apply2AllShapesC(auto const& func) const
	{
		for (Shape const& s : m_shapes)
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

    vector<Shape> m_shapes;
	CoordPos      m_initialPos;
};
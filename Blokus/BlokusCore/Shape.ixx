// Shape.ixx
//
// BlokusCore

export module BlokusCore:Shape;

import std;
import Types;
import Color;
import Direct2D;
import :CoordPos;

using std::array;
using std::vector;

export using SHAPE = array<array<bool,MAX_SHAPE_EXTENSION>, MAX_SHAPE_EXTENSION>;

export class Shape
{
public:
	Shape() = default;

	Shape(SHAPE const&);

	bool operator==(Shape const&) const = default;

	bool IsPartOfShape(CoordPos const& pos) const
	{
		return IsInShapeRange(pos) && m_shape[pos.GetYvalue()][pos.GetXvalue()];
	}

    void Apply2AllContactPnts(auto const& func) const
    {
        for (CoordPos const& pos : m_contactPnts)
            func(pos);
    }

	void Flip();
	void Rotate();

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

private:

    SHAPE            m_shape;
	vector<CoordPos> m_contactPnts;

	void colSquare        (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void shapeSquare      (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void drawShapeSquares (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void drawContactPoints(D2D_driver const &, fPixelPoint const, fPixel const) const;
	bool diagContact (CoordPos const&) const;
	bool orthoContact(CoordPos const&) const;
	bool spaceAtTop() const;
	bool spaceAtLeft() const;
	void shiftTop();
	void shiftLeft();
	void normalize();
};
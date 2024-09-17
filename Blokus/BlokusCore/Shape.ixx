// Shape.ixx
//
// BlokusCore

export module BlokusCore:Shape;

import std;
import Types;
import Color;
import Direct2D;
import :Pos;

using std::array;
using std::vector;

export using SHAPE = array<array<bool,MAX_SHAPE_EXTENSION>, MAX_SHAPE_EXTENSION>;

export class Shape
{
public:
	Shape() = default;

	Shape(SHAPE const&);

	bool operator==(Shape const&) const = default;

	bool IsPartOfShape(Pos const& pos) const
	{
		return pos.IsInShapeRange() && m_shape[pos.m_y.GetValue()][pos.m_x.GetValue()];
	}

    void Apply2AllContactPnts(auto const& func) const
    {
        for (Pos const& pos : m_contactPnts)
            func(pos);
    }

	void Flip();
	void Rotate();

	void Draw(D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;

private:

    SHAPE       m_shape;
	vector<Pos> m_contactPnts;

	void colSquare        (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void shapeSquare      (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void drawShapeSquares (D2D_driver const &, fPixelPoint const, Color const, fPixel const) const;
	void drawContactPoints(D2D_driver const &, fPixelPoint const, fPixel const) const;
	bool diagContact (Pos const&) const;
	bool orthoContact(Pos const&) const;
	bool spaceAtTop() const;
	bool spaceAtLeft() const;
	void shiftTop();
	void shiftLeft();
	void normalize();
};
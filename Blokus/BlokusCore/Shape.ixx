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

using ShapeCoordPos = CoordPos;

export class Shape
{
public:
	Shape() = default;

	Shape(SHAPE const&);

	void CollectCornerPnts();

	bool operator==(Shape const&) const = default;

    void Apply2AllCornerPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_cornerPnts)
            func(pos);
    }

	void Apply2AllShapeCellsC(auto const& func) const
	{
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_ROW; pos.IncX())
			if (isPartOfShape(pos))
				func(pos);
	}

	bool Apply2AllShapeCellsB(auto const& func) const
	{
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_ROW; pos.IncX())
			if (isPartOfShape(pos) && !func(pos))
				return false;
		return true;
	}

	void Flip();
	void Rotate();

	void Draw(D2D_driver const&, fPixelPoint const, Color const, fPixel const) const;

private:

    SHAPE                 m_shape;
	vector<ShapeCoordPos> m_cornerPnts;

	bool isCornerPnt  (CoordPos const&) const;
	bool isPartOfShape(ShapeCoordPos const&) const;

	void drawShapeSquares(D2D_driver const&, fPixelPoint const&, Color const, fPixel const) const;
	bool spaceAtTop() const;
	bool spaceAtLeft() const;
	void shiftTop();
	void shiftLeft();
	void normalize();
};
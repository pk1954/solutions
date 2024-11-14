// Shape.ixx
//
// BlokusCore

export module BlokusCore:Shape;

import std;
import Types;
import Color;
import SaveCast;
import DrawContext;
import :BlokusCoords;

using std::array;
using std::vector;

export using ShapeCells = array<array<bool,MAX_SHAPE_EXTENSION>, MAX_SHAPE_EXTENSION>;

export using ShapeCoordPos = CoordPos;

export class Shape
{
public:
	Shape() = default;
	Shape(ShapeCells const&);
	//bool operator==(Shape const&) const = default;

	bool Equals(Shape const &other) const
	{
		return m_shapeCells == other.m_shapeCells;
	}

	int  CountCells() const;
	void Draw(DrawContext&, Color const, bool const) const;
	void Draw(DrawContext&, Degrees const, Color const, bool const) const;

	void CollectCornerPnts();
	void Flip();
	void Rotate();

	Degrees GetRotation() const { return m_degRotation; }

    void Apply2AllCornerPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_cornerPnts)
            func(pos);
    }

	void Apply2AllShapeCellsC(auto const& func) const
	{
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_COL; pos.IncX())
			if (isPartOfShape(pos))
				func(pos);
	}

	bool Apply2AllShapeCellsB(auto const& func) const
	{
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_COL; pos.IncX())
			if (isPartOfShape(pos) && !func(pos))
				return false;
		return true;
	}

	MicroMeterPnt CenterOfGravity()
	{
		MicroMeterPnt umPntCenter { NP_ZERO };
		Apply2AllShapeCellsC
		(
			[&umPntCenter](ShapeCoordPos const &pos)
			{
				umPntCenter += Convert2fCoord(pos);
			}
		);
		umPntCenter /= Cast2Float(m_shapeCells.size());
		return umPntCenter;
	}

private:

    ShapeCells            m_shapeCells;
	Degrees               m_degRotation { 0._Degrees };
	MicroMeterPnt         m_umPntCenter;
	vector<ShapeCoordPos> m_cornerPnts;

	bool isCornerPnt  (CoordPos const&) const;
	bool isPartOfShape(ShapeCoordPos const&) const;

	bool spaceAtTop() const;
	bool spaceAtLeft() const;
	void shiftTop();
	void shiftLeft();
	void normalize();
};
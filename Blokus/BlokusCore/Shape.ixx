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

	MicroMeterPnt CenterOfGravity();

	Degrees GetRotation() const { return m_degRotation; }

    void Apply2AllCornerPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_cornerPnts)
            func(pos);
    }

    bool IsTrueForAnyCornerPnt(auto const& func) const
    {
        for (ShapeCoordPos const& pos : m_cornerPnts)
			if (func(pos))
				return true;
		return false;
    }

	void Apply2AllShapeCellsC(auto const& func) const
	{
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_COL; pos.IncX())
			if (isPartOfShape(pos))
				func(pos);
	}

	bool IsTrue4AllShapeCells(auto const& func) const   // returns true, if func delivers true 
	{                                                   // for all shape cells, else false
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_COL; pos.IncX())
			if (isPartOfShape(pos) && !func(pos))
				return false;
		return true;
	}

	bool IsTrue4AnyShapeCell(auto const& crit) const 
	{                                                  
		ShapeCoordPos pos;
		for (pos.SetY(0_COORD); pos.GetY() <= MAX_ROW; pos.IncY())
		for (pos.SetX(0_COORD); pos.GetX() <= MAX_COL; pos.IncX())
			if (isPartOfShape(pos) && crit(pos))
				return true;
		return false;
	}

	bool IsCompletelyOnBoard(CoordPos const&) const;

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
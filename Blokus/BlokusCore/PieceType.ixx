// PieceType.ixx
//
// BlokusCore

export module BlokusCore:PieceType;

import std;
import Debug;
import SaveCast;
import Types;
import Color;
import DrawContext;
import :Shape;
import :ShapeId;
import :BlokusCoords;

using std::vector;

export class PieceType
{
public:

	void SetShape(const ShapeCells&);

	void SetPos(int const, int const);
	void SetPos(CoordPos const& newPos) { m_initialPos = newPos; }

	CoordPos GetInitialPos() const { return m_initialPos; }

	Shape const& GetShapeC(ShapeId const id) const { return m_shapes.at(id.GetValue()); }

	void Draw(DrawContext&, ShapeId const, PosDir        const&, Color const, bool const) const;
	void Draw(DrawContext&, ShapeId const, MicroMeterPnt const&, Color const, bool const) const;

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

	unsigned int NrOfCells() const { return m_iNrOfCells; }

private:
	void initialize(ShapeCells const&);
	void addIfNew  (Shape const&);
	void addOrientations(Shape&);

	unsigned int  m_iNrOfCells { 0 };
    vector<Shape> m_shapes;
	CoordPos      m_initialPos;
};
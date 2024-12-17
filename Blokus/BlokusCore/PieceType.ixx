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
import :PieceTypeId;

using std::vector;
using std::wstring;

export class PieceType
{
public:
	void SetPieceTypeId(PieceTypeId const id) { m_idPieceType = id; }

	void SetShape(const ShapeCells&);

	void SetPos(int const, int const);
	void SetPos(CoordPos const& newPos) { m_initialPos = newPos; }

	CoordPos GetInitialPos() const { return m_initialPos; }

	Shape const& GetShapeC(ShapeId const id) const { return m_shapes.at(id.GetValue()); }

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

	int NrOfCells () const { return m_iNrOfCells; }
	int NrOfShapes() const { return Cast2Int(m_shapes.size()); }

private:
	void addIfNew  (Shape const&);
	void addOrientations(Shape&);

	PieceTypeId   m_idPieceType { UndefinedPieceTypeId };
	unsigned int  m_iNrOfCells { 0 };
    vector<Shape> m_shapes;
	CoordPos      m_initialPos;
};
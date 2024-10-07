// BlokusUtilities.ixx
//
// BlokusCore

export module BlokusCore:BlokusUtilities;

import std;
import Types;
import Color;
import Direct2D;
import DrawContext;
import :BlokusCoords;
import :Components;
import :PieceType;
import :Move;
import :Shape;

MicroMeterPnt GetCenter(CoordPos const& coordPos)
{
	MicroMeterPnt const umPos       { Convert2fCoord(coordPos) };
	MicroMeter    const umHalfSize  { UM_CELL_SIZE * 0.5f };
	MicroMeterPnt const umPosCenter { umPos + MicroMeterPnt(umHalfSize) };
	return umPosCenter;
}

Color darker(Color const col)  { return col * 0.6f; }

export void SmallDot
(
	DrawContext const &context,
	CoordPos    const &pos,
	Color       const  col
)
{
	context.FillCircle(MicroMeterCircle(GetCenter(pos), UM_CELL_SIZE * 0.3f), col);
	context.FillCircle(MicroMeterCircle(GetCenter(pos), UM_CELL_SIZE * 0.2f), darker(col));
}

void colSquare
(
	DrawContext   const &context, 
	MicroMeterPnt const  center,
	Color         const  col,
	MicroMeter    const  umHalfSize
)
{
	MicroMeterPnt const umPntStartPos { center - MicroMeterPnt(umHalfSize) };
	MicroMeterPnt const umPntEndPos   { center + MicroMeterPnt(umHalfSize) };
	context.FillRectangle(MicroMeterRect(umPntStartPos, umPntEndPos), col);
}

export void ShapeSquare
(
	DrawContext const &context, 
	CoordPos    const &coordPos,
	Color       const  col
)
{
	MicroMeterPnt const umPos       { Convert2fCoord(coordPos) };
	MicroMeter    const umHalfSize  { UM_CELL_SIZE * 0.5f };
	MicroMeterPnt const umPosCenter { umPos + MicroMeterPnt(umHalfSize) };
	colSquare(context, umPosCenter, col,         umHalfSize       );
	colSquare(context, umPosCenter, darker(col), umHalfSize * 0.8f);
}

Shape const& GetShapeC(Move const &move)
{
    PieceType const& pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
    Shape     const& shape     { pieceType  .GetShapeC    (move.GetShapeId()) };
    return shape;
}

// BlokusUtilities.ixx
//
// BlokusCore

export module BlokusCore:BlokusUtilities;

import std;
import Types;
import Color;
import Direct2D;
import DrawContext;
import :BlokusMove;
import :BlokusCoords;
import :Components;
import :Piece;
import :PieceType;
import :PlayerId;
import :Shape;

MicroMeterPnt GetCenter(CoordPos const& coordPos)
{
	MicroMeterPnt const umPos       { Convert2fCoord(coordPos) };
	MicroMeter    const umHalfSize  { UM_CELL_SIZE * 0.5f };
	MicroMeterPnt const umPosCenter { umPos + MicroMeterPnt(umHalfSize) };
	return umPosCenter;
}

export bool IsInShapeCell
(
	MicroMeterPnt const &umPos,
	MicroMeterPnt const &umPosCell
)
{
	MicroMeterRect const umRect { umPosCell, UM_CELL_SIZE };
	return umRect.Includes(umPos);
}

export bool IsInShapeCell
(
	MicroMeterPnt const &umPos,
	CoordPos      const &coordPos
)
{
	MicroMeterPnt const umPosCell { Convert2fCoord(coordPos) };
	return IsInShapeCell(umPos, umPosCell);
}

export bool IsInPiece
(
	MicroMeterPnt const &umPos,
	Piece         const &piece
)
{
	PieceType     const &pieceType    { piece.GetPieceTypeC() };
	Shape         const &shape        { pieceType.GetShapeC(ShapeId(0)) };
	CoordPos      const coordPosPiece { pieceType.GetInitialPos() };
	MicroMeterPnt const umPosPiece    { Convert2fCoord(coordPosPiece) };
	MicroMeterPnt const umP           { umPos - umPosPiece };
	return shape.IsTrueForAnyShapeCell([&umP](ShapeCoordPos const &cp){ return IsInShapeCell(umP, cp); });
}

export ShapeCoordPos FindCell
(
	MicroMeterPnt const &umPos,
	Piece         const &piece
)
{
	PieceType     const &pieceType    { piece.GetPieceTypeC() };
	Shape         const &shape        { pieceType.GetShapeC(ShapeId(0)) };
	CoordPos      const coordPosPiece { pieceType.GetInitialPos() };
	MicroMeterPnt const umPosPiece    { Convert2fCoord(coordPosPiece) };
	MicroMeterPnt const umP           { umPos - umPosPiece };
	return shape.FindShapeCell([&umP](ShapeCoordPos const &cp){return IsInShapeCell(umP, cp);});
}

Color darker  (Color const col)  { return col * 0.6f; }
Color brighter(Color const col)  { return col * 2.0f; }

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
	Color       const  col,
	bool        const  bHighlighted
)
{
	MicroMeterPnt const umPos       { Convert2fCoord(coordPos) };
	MicroMeter    const umHalfSize  { UM_CELL_SIZE * 0.5f };
	MicroMeterPnt const umPosCenter { umPos + MicroMeterPnt(umHalfSize) };
	colSquare(context, umPosCenter, bHighlighted ? brighter(col) : col,         umHalfSize       );
	colSquare(context, umPosCenter, bHighlighted ? col           : darker(col), umHalfSize * 0.8f);
}

Shape const& GetShapeC(BlokusMove const &move)
{
    PieceType const& pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
    Shape     const& shape     { pieceType  .GetShapeC    (move.GetShapeId()) };
    return shape;
}

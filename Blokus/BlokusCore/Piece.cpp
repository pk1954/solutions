// Piece.cpp
//
// BlokusCore

module BlokusCore:Piece;

void Piece::Draw
(
	DrawContext  &context,
	PosDir const &posDir, 
	Color  const  col,
	bool   const  bHighlighted
) const
{
	fPixelPoint const offsetSave { context.GetPixelOffset() };
	context.Move(posDir.m_umPos);
	Shape const& shape { GetPieceTypeC().GetShapeC(m_idShape) };
	shape.Draw(context, posDir.m_degrees, col, bHighlighted, m_hTextFormat);
	context.SetPixelOffset(offsetSave);
}

void Piece::Draw
(
	DrawContext         &context,
	MicroMeterPnt const &umPos, 
	Color         const  col,
	bool          const  bHighlighted,
	ShapeId       const  idShape
) const
{
	fPixelPoint const offsetSave { context.GetPixelOffset() };
	context.Move(umPos);
	Shape const& shape { GetPieceTypeC().GetShapeC(idShape) };
	shape.Draw(context, col, bHighlighted, m_hTextFormat);
	context.SetPixelOffset(offsetSave);
}

// Piece.cpp
//
// BlokusCore

module BlokusCore:Piece;

void Piece::Draw
(
	DrawContext            &context,
	PosDir           const &posDir, 
	Color            const  col,
	bool             const  bHighlighted,
	TextFormatHandle const  hTextFormat
) const
{
	fPixelPoint const offsetSave { context.GetPixelOffset() };
	context.Move(posDir.m_umPos);
	Shape const& shape { GetPieceTypeC().GetShapeC(m_idShape) };
	shape.Draw(context, posDir.m_degrees, col, bHighlighted, hTextFormat);
	context.SetPixelOffset(offsetSave);
}

//void Piece::Draw
//(
//	DrawContext            &context,
//	MicroMeterPnt    const &umPos, 
//	Color            const  col,
//	bool             const  bHighlighted,
//	TextFormatHandle const  hTextFormat
//) const
//{
//	Draw(context, umPos, col, bHighlighted, hTextFormat, m_idShape);
//}

void Piece::Draw
(
	DrawContext            &context,
	MicroMeterPnt    const &umPos, 
	Color            const  col,
	bool             const  bHighlighted,
	TextFormatHandle const  hTextFormat,
	ShapeId          const  idShape
) const
{
	fPixelPoint const offsetSave { context.GetPixelOffset() };
	context.Move(umPos);
	Shape const& shape { GetPieceTypeC().GetShapeC(idShape) };
	shape.Draw(context, col, bHighlighted, hTextFormat);
	context.SetPixelOffset(offsetSave);
}

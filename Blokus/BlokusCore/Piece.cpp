// Piece.cpp
//
// BlokusCore

module BlokusCore:Piece;

void Piece::Initialize(PieceTypeId const id)
{
    m_idPieceType = id;
    Reset();
    if (m_hTextFormat == nullptr)
        m_hTextFormat = D2D_driver::NewTextFormat(12.f);
}

void Piece::Reset()
{
    m_bAvailable = true;
    setPiecePos(GetPieceTypeC().GetInitialPos());
    m_idShape = ShapeId(0);
    m_posDir.m_degrees = 0._Degrees;
}

void Piece::DoMove(BlokusMove const& move)
{
    m_bAvailable = false;
    m_idShape = move.GetShapeId();
    setPiecePos(move.GetCoordPos());
}

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
	shape.Draw(context, posDir.m_degrees, col, bHighlighted);
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
	shape.Draw(context, col, bHighlighted);
	context.SetPixelOffset(offsetSave);
}

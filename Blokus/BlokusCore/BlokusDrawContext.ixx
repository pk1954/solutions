// BlokusDrawContext.ixx
//
// BlokusCore

export module BlokusCore:BlokusDrawContext;

import std;
import Types;
import Color;
import Direct2D;
import :BlokusCoords;
import :BlokusCoordSys;

using std::wstring;

export class BlokusDrawContext
{
public:
	void Start(D2D_driver * const pGraphics)
	{
		m_pGraphics = pGraphics;
	}

	void Reset(fPixelPoint const& offset, fPixel const cellSize)
	{
		m_coord.Reset(offset, cellSize);
	}

	fPixel             CellSize() const { return m_coord.CellSize(); }
	fPixelPoint const& Offset  () const { return m_coord.Offset(); }

	fPixelPoint SetOffset (fPixelPoint    const &o) { return m_coord.SetOffset(o); }
	fPixelPoint Add2Offset(BlokusCoordPos const &o) { return m_coord.Add2Offset(o); }

    fPixelPoint GetCenter (BlokusCoordPos const &p) const { return m_coord.GetCenter(p); }

	void DrawLine
    (
        BlokusCoordPos const &startPos,
        BlokusCoordPos const &endPos,
        Color          const  color
    ) const
    {
		m_pGraphics->DrawLine
		(
			m_coord.Transform2fPixelPos(startPos),
			m_coord.Transform2fPixelPos(endPos),
			1.0_fPixel,
			color
		);
    }

	void DisplayText
	(
		BlokusCoordRect  const &rect,
		wstring          const &text,
		TextFormatHandle const hTextFormat
	)
	{
		m_pGraphics->DisplayText(m_coord.Transform2fPixelRect(rect), text, hTextFormat);
	}

	void SmallDot
	(
		BlokusCoordPos const &pos,
		Color          const  col
	)
	{
		fPixelPoint const center { GetCenter(pos) };
		fPixel      const size   { CellSize() };
		m_pGraphics->FillCircle(fPixelCircle(center, size * 0.2f), col);
	}

	void ShapeSquare
	(
		BlokusCoordPos const &pos,
		Color          const  col
	)
	{
		fPixelPoint const center { GetCenter(pos) };
		fPixel      const size   { CellSize() };
		colSquare(center,  col,        size       );
		colSquare(center,  col * 0.6f, size * 0.8f);
	}

private:
    D2D_driver   * m_pGraphics { nullptr };
    BlokusCoordSys m_coord     { };

	void colSquare
	(
		fPixelPoint const center,
		Color       const col,
		fPixel      const size
	)
	{
		fPixel         const fPixHalfSize  { size * 0.5f };
		fPixelPoint    const fPixPntOrigin { center - fPixelPoint(fPixHalfSize, fPixHalfSize) };
		fPixelRectSize const fPixSize      { fPixelRectSize(size, size) };
		m_pGraphics->FillRectangle(fPixelRect(fPixPntOrigin, fPixSize), col);
	}
};
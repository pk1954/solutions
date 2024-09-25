// D2D_DrawContext.cpp
//
// Win32_utilities

module D2D_DrawContext;

import std;
import Direct2D;
import Color;
import Types;

using std::wstring;
using std::max;

void D2D_DrawContext::Start(D2D_driver * const pGraphics)
{
	m_pGraphics = pGraphics;
	Initialize();
	SetStdFontSize(STD_FONT_SIZE);
}

void D2D_DrawContext::SetStdFontSize(MicroMeter const & size)
{
	m_pGraphics->SetStdFontSize(m_coord.Transform2fPixel(size).GetValue());
}

void D2D_DrawContext::SetRotation(Degrees const degrees, MicroMeterPnt const& umPntCenter) const
{
	m_pGraphics->SetRotation(degrees.GetValue(), m_coord.Transform2fPixelPos(umPntCenter));
}

void D2D_DrawContext::Reset() const
{
	m_pGraphics->Reset();
}

void D2D_DrawContext::DrawLine
(
	MicroMeterPnt const &umStartPoint,
	MicroMeterPnt const &umEndPoint,
	MicroMeter    const  umWidth,
	Color         const  col,
	fPixel        const  fPixMin
) const
{
	if (umStartPoint != umEndPoint)
	{
		fPixel const fPixWidth { max(m_coord.Transform2fPixel(umWidth), fPixMin) };
		m_pGraphics->DrawLine
		(
			m_coord.Transform2fPixelPos(umStartPoint),
			m_coord.Transform2fPixelPos(umEndPoint),
			fPixWidth,
			m_bNoColors ? COL_BLACK : col
		);
	}
}

void D2D_DrawContext::DrawLine
(
	MicroMeterLine const & umLine, 
	MicroMeter     const   umWidth, 
	Color          const   col
) const
{
	DrawLine(umLine.GetStartPoint(), umLine.GetEndPoint(), umWidth, col	);
}

void D2D_DrawContext::FillCircle
(
	MicroMeterCircle const & umC,
	Color            const   col,
	fPixel           const   fPixMin
) const
{
	fPixel       const fPixRadius { max(m_coord.Transform2fPixel   (umC.GetRadius()), fPixMin) };
	fPixelCircle const fPixCircle (     m_coord.Transform2fPixelPos(umC.GetPos   ()), fPixRadius);
	m_pGraphics->FillCircle(fPixCircle, m_bNoColors ? COL_BLACK : col);
}

void D2D_DrawContext::FillGradientCircle
(
	MicroMeterCircle const & umCircle,
	Color            const   col1,  
	Color            const   col2  
) const
{
	m_pGraphics->FillGradientCircle(m_coord.Transform2fPixelCircle(umCircle), col1, col2);
}

void D2D_DrawContext::DrawCircle
(
	MicroMeterCircle const& umCircle,
	BrushHandle      const  hBrush,
	fPixel           const  fPixWidth
) const
{
	m_pGraphics->DrawCircle(m_coord.Transform2fPixelCircle(umCircle), hBrush, fPixWidth);
}

void D2D_DrawContext::DrawCircle
(
	MicroMeterCircle const& umCircle,
	Color            const  col,
	fPixel           const  fPixWidth
) const
{
	m_pGraphics->DrawCircle(m_coord.Transform2fPixelCircle(umCircle), col, fPixWidth);
}

void D2D_DrawContext::DrawCircle
(
	MicroMeterCircle const& umCircle, 
	fPixel           const  fPixWidth
) const
{
	m_pGraphics->DrawCircle(m_coord.Transform2fPixelCircle(umCircle), fPixWidth);
}

void D2D_DrawContext::FillEllipse
(
	MicroMeterEllipse const& umEllipse,
	BrushHandle       const  hBrush
) const
{
	m_pGraphics->FillEllipse(m_coord.Transform2fPixelEllipse(umEllipse), hBrush);
}

void D2D_DrawContext::FillEllipse
(
	MicroMeterEllipse const& umEllipse,
	Color             const   col
) const
{
	m_pGraphics->FillEllipse
	(
		m_coord.Transform2fPixelEllipse(umEllipse),
		m_bNoColors ? COL_BLACK : col
	);
}

void D2D_DrawContext::FillEllipse(MicroMeterEllipse const& umEllipse) const
{
	m_pGraphics->FillEllipse(m_coord.Transform2fPixelEllipse(umEllipse));
}

void D2D_DrawContext::DrawEllipse
(
	MicroMeterEllipse const & umEllipse,
	Color             const   col,
	MicroMeter        const   umWidth
) const
{
	m_pGraphics->DrawEllipse
	(
		m_coord.Transform2fPixelEllipse(umEllipse), 
		m_bNoColors ? COL_BLACK : col,
		m_coord.Transform2fPixel(umWidth)
	);
}

void D2D_DrawContext::DrawBezier
(
	MicroMeterPnt const& umPnt0,
	MicroMeterPnt const& umPnt1,
	MicroMeterPnt const& umPnt2,
	MicroMeterPnt const& umPnt3,
	BrushHandle          hBrush,
	fPixel        const  fPixWidth
) const
{
	m_pGraphics->DrawBezier
	(
		m_coord.Transform2fPixelPos(umPnt0),
		m_coord.Transform2fPixelPos(umPnt1),
		m_coord.Transform2fPixelPos(umPnt2),
		m_coord.Transform2fPixelPos(umPnt3),
		hBrush,
		fPixWidth
	);
}

void D2D_DrawContext::DrawBezier
(
	MicroMeterPnt const& umPnt0,
	MicroMeterPnt const& umPnt1,
	MicroMeterPnt const& umPnt2,
	MicroMeterPnt const& umPnt3,
	Color         const  col,
	fPixel        const  fPixWidth
) const
{
	m_pGraphics->DrawBezier
	(
		m_coord.Transform2fPixelPos(umPnt0),
		m_coord.Transform2fPixelPos(umPnt1),
		m_coord.Transform2fPixelPos(umPnt2),
		m_coord.Transform2fPixelPos(umPnt3),
		col,
		fPixWidth
	);
}

void D2D_DrawContext::DrawBezier
(
	MicroMeterPnt const& umPnt0,
	MicroMeterPnt const& umPnt1,
	MicroMeterPnt const& umPnt2,
	MicroMeterPnt const& umPnt3,
	fPixel        const  fPixWidth
) const
{
	m_pGraphics->DrawBezier
	(
		m_coord.Transform2fPixelPos(umPnt0),
		m_coord.Transform2fPixelPos(umPnt1),
		m_coord.Transform2fPixelPos(umPnt2),
		m_coord.Transform2fPixelPos(umPnt3),
		fPixWidth
	);
}

void D2D_DrawContext::FillArrow
(
	MicroMeterPnt const & umPos, 
	MicroMeterPnt const & umVector, 
	MicroMeter    const   umSize, 
	MicroMeter    const   umWidth, 
	Color         const   col
) const
{
	m_pGraphics->FillArrow
	(
		m_coord.Transform2fPixelPos (umPos),
		m_coord.Transform2fPixelSize(umVector), 
		m_coord.Transform2fPixel    (umSize),
		m_coord.Transform2fPixel    (umWidth),
		m_bNoColors ? COL_BLACK : col 
	);
}

void D2D_DrawContext::FillRectangle(MicroMeterRect const& umRect, BrushHandle const hBrush) const
{
	m_pGraphics->FillRectangle(m_coord.Transform2fPixelRect(umRect), hBrush);
}

void D2D_DrawContext::FillRectangle(MicroMeterRect const& umRect, Color col) const
{
	m_pGraphics->FillRectangle(m_coord.Transform2fPixelRect(umRect), col);
}

void D2D_DrawContext::FillRoundedRectangle(MicroMeterRect const& umRect, Color col, MicroMeter const umRadius) const
{
	m_pGraphics->FillRoundedRectangle(m_coord.Transform2fPixelRect(umRect), col, m_coord.Transform2fPixel(umRadius));
}

void D2D_DrawContext::DrawTranspRect(MicroMeterRect const & umRect, Color col) const
{
	if (IsTooSmall(umRect))
	{
		m_pGraphics->FillRectangle
		(
			fPixelRect
			(
				fPP_ZERO,
				m_coord.Transform2fPixelPos(umRect.GetEndPoint()) 
			), 
			col 
		);
		m_pGraphics->FillRectangle
		(
			fPixelRect
			(
				m_coord.Transform2fPixelPos(umRect.GetStartPoint()), 
				m_pGraphics->GetClRectSize() 
			), 
			col 
		);
	}
	else
	{
		m_pGraphics->FillRectangle(m_coord.Transform2fPixelRect(umRect), col);
	}
}

void D2D_DrawContext::DisplayText
(
	MicroMeterRect   const & umRect,
	wstring          const & wstr,
	TextFormatHandle const   hTextFormat
) const
{
	m_pGraphics->DisplayText(m_coord.Transform2fPixelRect(umRect), wstr, hTextFormat);
}

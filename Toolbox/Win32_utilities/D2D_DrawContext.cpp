// DrawContext.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "d2d1helper.h"
#include "D2D_DrawContext.h"

void D2D_DrawContext::Start(HWND const hwnd)
{
	m_graphics.Initialize(hwnd);
	DrawContext::Initialize();
	SetStdFontSize(STD_FONT_SIZE);
}

void D2D_DrawContext::Stop()
{
	m_graphics.ShutDown();
}

bool D2D_DrawContext::StartFrame() 
{ 
	return m_graphics.StartFrame(); 
}

void D2D_DrawContext::EndFrame() 
{ 
	m_graphics.EndFrame(); 
}

void D2D_DrawContext::Resize(int const width, int const height)
{
	m_graphics.Resize(width, height);
}

void D2D_DrawContext::SetStdFontSize(MicroMeter const & size)
{
	m_graphics.SetStdFontSize(m_coord.Transform2fPixel(size).GetValue());
}

void D2D_DrawContext::DrawLine
(
	MicroMeterPnt const & umStartPoint, 
	MicroMeterPnt const & umEndPoint, 
	MicroMeter    const   umWidth, 
	ColorF        const   col,
	fPixel        const   fPixMin
) const
{
	if (umStartPoint != umEndPoint)
	{
		fPixel const fPixWidth { max(m_coord.Transform2fPixel(umWidth), fPixMin) };
		m_graphics.DrawLine
		(
			m_coord.Transform2fPixelPos(umStartPoint),
			m_coord.Transform2fPixelPos(umEndPoint  ),
			fPixWidth,
			m_bNoColors ? ColorF::Black : col 
		);
	}
}

void D2D_DrawContext::DrawLine
(
	MicroMeterLine const & umLine, 
	MicroMeter     const   umWidth, 
	ColorF         const   col
) const
{
	DrawLine(umLine.GetStartPoint(), umLine.GetEndPoint(), umWidth, col	);
}

void D2D_DrawContext::FillCircle
(
	MicroMeterCircle const & umC,
	ColorF           const   col,
	fPixel           const   fPixMin
) const
{
	fPixel       const fPixRadius { max(m_coord.Transform2fPixel   (umC.GetRadius()), fPixMin) };
	fPixelCircle const fPixCircle (     m_coord.Transform2fPixelPos(umC.GetPos   ()), fPixRadius);
	m_graphics.FillCircle(fPixCircle, m_bNoColors ? ColorF::Black : col);
}

void D2D_DrawContext::FillGradientCircle
(
	MicroMeterCircle const & umCircle,
	ColorF           const   col1,  
	ColorF           const   col2  
) const
{
	m_graphics.FillGradientCircle(m_coord.Transform2fPixelCircle(umCircle), col1, col2);
}

void D2D_DrawContext::DrawCircle
(
	MicroMeterCircle const & umCircle,
	ColorF           const   col,
	MicroMeter       const   umWidth
) const
{
	m_graphics.DrawCircle
	(
		m_coord.Transform2fPixelCircle(umCircle), 
		m_bNoColors ? ColorF::Black : col,
		m_coord.Transform2fPixel(umWidth)
	);
}

void D2D_DrawContext::FillEllipse
(
	MicroMeterEllipse const & umEllipse,
	ColorF            const   col  
) const
{
	m_graphics.FillEllipse
	(
		m_coord.Transform2fPixelEllipse(umEllipse), 
		m_bNoColors ? ColorF::Black : col 
	);
}

void D2D_DrawContext::DrawEllipse
(
	MicroMeterEllipse const & umEllipse,
	ColorF            const   col,
	MicroMeter        const   umWidth
) const
{
	m_graphics.DrawEllipse
	(
		m_coord.Transform2fPixelEllipse(umEllipse), 
		m_bNoColors ? ColorF::Black : col,
		m_coord.Transform2fPixel(umWidth)
	);
}

void D2D_DrawContext::FillArrow
(
	MicroMeterPnt const & umPos, 
	MicroMeterPnt const & umVector, 
	MicroMeter    const   umSize, 
	MicroMeter    const   umWidth, 
	ColorF        const   col
) const
{
	m_graphics.FillArrow
	(
		m_coord.Transform2fPixelPos (umPos),
		m_coord.Transform2fPixelSize(umVector), 
		m_coord.Transform2fPixel    (umSize),
		m_coord.Transform2fPixel    (umWidth),
		m_bNoColors ? ColorF::Black : col 
	);
}

void D2D_DrawContext::FillRectangle(MicroMeterRect const & umRect, ColorF col) const 
{
	m_graphics.FillRectangle(m_coord.Transform2fPixelRect(umRect), col);
}

void D2D_DrawContext::DrawTranspRect(MicroMeterRect const & umRect, ColorF col) const 
{
	if (IsTooSmall(umRect))
	{
		m_graphics.FillRectangle
		(
			fPixelRect
			(
				fPP_ZERO,
				m_coord.Transform2fPixelPos(umRect.GetEndPoint()) 
			), 
			col 
		);
		m_graphics.FillRectangle
		(
			fPixelRect
			(
				m_coord.Transform2fPixelPos(umRect.GetStartPoint()), 
				m_graphics.GetClRectSize() 
			), 
			col 
		);
	}
	else
	{
		m_graphics.FillRectangle(m_coord.Transform2fPixelRect(umRect), col);
	}
}

void D2D_DrawContext::DisplayText
(
	MicroMeterRect      const & umRect,
	wstring             const & wstr,
	ColorF              const   colF,
	IDWriteTextFormat * const   pTextFormat
) const
{
	m_graphics.DisplayText(m_coord.Transform2fPixelRect(umRect), wstr, colF, pTextFormat);
}

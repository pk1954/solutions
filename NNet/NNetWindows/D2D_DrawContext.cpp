// DrawContext.cpp
//
// NNetWindows

#include "stdafx.h"
#include "d2d1helper.h"
#include "D2D_DrawContext.h"

void D2D_DrawContext::Start( D2D_driver * const pGraphics )
{
	DrawContext::Initialize();
	m_pGraphics = pGraphics;
	SetStdFontSize( STD_FONT_SIZE );
}

void D2D_DrawContext::Stop()
{
	m_pGraphics->ShutDown();
}

void D2D_DrawContext::Resize( int const width, int const height )
{
	m_pGraphics->Resize( width, height );
}

void D2D_DrawContext::SetStdFontSize( MicroMeter const & size )
{
	m_pGraphics->SetStdFontSize( m_coord.Transform2fPixel( size ).GetValue() );
}

void D2D_DrawContext::DrawLine
( 
	MicroMeterPnt const & umStartPoint, 
	MicroMeterPnt const & umEndPoint, 
	MicroMeter      const   umWidth, 
	D2D1::ColorF    const   col
) const
{
	if ( umStartPoint != umEndPoint )
	{
		m_pGraphics->DrawLine
		( 
			m_coord.Transform2fPixelPos( umStartPoint ),
			m_coord.Transform2fPixelPos( umEndPoint   ),
			m_coord.Transform2fPixel   ( umWidth      ),
			m_bNoColors ? D2D1::ColorF::Black : col 
		);
	}
}

void D2D_DrawContext::DrawLine
( 
	MicroMeterLine const & umLine, 
	MicroMeter     const   umWidth, 
	D2D1::ColorF   const   col
) const
{
	DrawLine(umLine.GetStartPoint(), umLine.GetEndPoint(), umWidth, col	);
}

void D2D_DrawContext::FillCircle
(
	MicroMeterCircle const & umCircle,
	D2D1::ColorF     const   col  
) const
{
	m_pGraphics->FillCircle
	( 
		m_coord.Transform2fPixelCircle( umCircle ), 
		m_bNoColors ? D2D1::ColorF::Black : col 
	);
}

void D2D_DrawContext::FillGradientCircle
(
	MicroMeterCircle const & umCircle,
	D2D1::ColorF     const   col1,  
	D2D1::ColorF     const   col2  
) const
{
	m_pGraphics->FillGradientCircle( m_coord.Transform2fPixelCircle( umCircle ), col1, col2 );
}

void D2D_DrawContext::DrawCircle
(
	MicroMeterCircle const & umCircle,
	D2D1::ColorF     const   col,
	MicroMeter       const   umWidth
) const
{
	m_pGraphics->DrawCircle
	( 
		m_coord.Transform2fPixelCircle( umCircle ), 
		m_bNoColors ? D2D1::ColorF::Black : col,
		m_coord.Transform2fPixel( umWidth )
	);
}

void D2D_DrawContext::FillEllipse
(
	MicroMeterEllipse const & umEllipse,
	D2D1::ColorF      const   col  
) const
{
	m_pGraphics->FillEllipse
	( 
		m_coord.Transform2fPixelEllipse( umEllipse ), 
		m_bNoColors ? D2D1::ColorF::Black : col 
	);
}

void D2D_DrawContext::DrawEllipse
(
	MicroMeterEllipse const & umEllipse,
	D2D1::ColorF     const   col,
	MicroMeter       const   umWidth
) const
{
	m_pGraphics->DrawEllipse
	( 
		m_coord.Transform2fPixelEllipse( umEllipse ), 
		m_bNoColors ? D2D1::ColorF::Black : col,
		m_coord.Transform2fPixel( umWidth )
	);
}

void D2D_DrawContext::FillArrow
( 
	MicroMeterPnt const & umPos, 
	MicroMeterPnt const & umVector, 
	MicroMeter      const   umSize, 
	MicroMeter      const   umWidth, 
	D2D1::ColorF    const   col
) const
{
	m_pGraphics->FillArrow
	( 
		m_coord.Transform2fPixelPos ( umPos ),
		m_coord.Transform2fPixelSize( umVector ), 
		m_coord.Transform2fPixel    ( umSize ),
		m_coord.Transform2fPixel    ( umWidth ),
		m_bNoColors ? D2D1::ColorF::Black : col 
	);
}

void D2D_DrawContext::FillRectangle( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
{
	m_pGraphics->FillRectangle( m_coord.Transform2fPixelRect( umRect ), col );
}

void D2D_DrawContext::DrawTranspRect( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
{
	if ( IsTooSmall( umRect ) )
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
		m_pGraphics->FillRectangle( m_coord.Transform2fPixelRect(umRect), col );
	}
}

void D2D_DrawContext::DisplayText
(
	MicroMeterRect      const & umRect,
	std::wstring        const & wstr,
	D2D1::ColorF        const   colF,
	IDWriteTextFormat * const   pTextFormat
) const
{
	m_pGraphics->DisplayText( m_coord.Transform2PixelRect( umRect ), wstr, colF, pTextFormat );
}

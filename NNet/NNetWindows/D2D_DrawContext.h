// D2D_DrawContext.h
//
// NNetWindows

#pragma once

#include "NNetColors.h"
#include "DrawContext.h"
#include "Direct2D.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "scale.h"

inline static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

class D2D_DrawContext: public DrawContext
{
public:

	void Start( HWND const hwnd )
	{
		DrawContext::Initialize( );
		m_graphics.Initialize( hwnd );
		SetStdFontSize( STD_FONT_SIZE );
		m_scale.Initialize( & m_graphics, L"m" );
	}

	void Stop( )
	{
		m_graphics.ShutDown( );
	}

	bool StartFrame( HDC const hDC )
	{
		return m_graphics.StartFrame( hDC );
	}

	void EndFrame( )
	{
		m_graphics.EndFrame( );
	}

	void Resize( int const width, int const height )
	{
		m_graphics.Resize( width, height );
		m_scale.SetClientRectSize( PIXEL(width), PIXEL(height) );
	}

	void SetStdFontSize( MicroMeter const & size )
	{
		m_graphics.SetStdFontSize( m_coord.Convert2fPixel( size ).GetValue() );
	}

	virtual void DrawLine
	( 
		MicroMeterPoint const & umStartPoint, 
		MicroMeterPoint const & umEndPoint, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		m_graphics.DrawLine
		( 
			m_coord.Convert2fPixelPos( umStartPoint ),
			m_coord.Convert2fPixelPos( umEndPoint   ),
			m_coord.Convert2fPixel   ( umWidth      ),
			m_bNoColors ? NNetColors::COL_BLACK : col 
		);
	}

	virtual void FillCircle
	(
		MicroMeterCircle const & umCircle,
		D2D1::ColorF     const   col  
	) const
	{
		m_graphics.FillCircle
		( 
			m_coord.Convert2fPixelCircle( umCircle ), 
			m_bNoColors ? NNetColors::COL_BLACK : col 
		);
	}

	virtual void FillGradientCircle
	(
		MicroMeterCircle const & umCircle,
		D2D1::ColorF     const   col1,  
		D2D1::ColorF     const   col2  
	) const
	{
		m_graphics.FillGradientCircle( m_coord.Convert2fPixelCircle( umCircle ), col1, col2 );
	}

	virtual void DrawCircle
	(
		MicroMeterCircle const & umCircle,
		D2D1::ColorF     const   col,
		MicroMeter       const   umWidth
	) const
	{
		m_graphics.DrawCircle
		( 
			m_coord.Convert2fPixelCircle( umCircle ), 
			m_bNoColors ? NNetColors::COL_BLACK : col,
			m_coord.Convert2fPixel( umWidth )
		);
	}

	virtual void FillEllipse
	(
		MicroMeterEllipse const & umEllipse,
		D2D1::ColorF      const   col  
	) const
	{
		m_graphics.FillEllipse
		( 
			m_coord.Convert2fPixelEllipse( umEllipse ), 
			m_bNoColors ? NNetColors::COL_BLACK : col 
		);
	}

	virtual void DrawEllipse
	(
		MicroMeterEllipse const & umEllipse,
		D2D1::ColorF     const   col,
		MicroMeter       const   umWidth
	) const
	{
		m_graphics.DrawEllipse
		( 
			m_coord.Convert2fPixelEllipse( umEllipse ), 
			m_bNoColors ? NNetColors::COL_BLACK : col,
			m_coord.Convert2fPixel( umWidth )
		);
	}

	virtual void FillArrow
	( 
		MicroMeterPoint const & umPos, 
		MicroMeterPoint const & umVector, 
		MicroMeter      const   umSize, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		m_graphics.FillArrow
		( 
			m_coord.Convert2fPixelPos ( umPos ),
			m_coord.Convert2fPixelSize( umVector ), 
			m_coord.Convert2fPixel    ( umSize ),
			m_coord.Convert2fPixel    ( umWidth ),
			m_bNoColors ? NNetColors::COL_BLACK : col 
		);
	}

	virtual void FillRectangle( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
	{
		m_graphics.FillRectangle( m_coord.Convert2fPixelRect( umRect ), col );
	}

	virtual void DrawTranspRect( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
	{
		if ( IsTooSmall( umRect ) )
		{
			m_graphics.FillRectangle
			( 
				fPixelRect
				( 
					fPP_ZERO,
					m_coord.Convert2fPixelPos(umRect.GetEndPoint()) 
				), 
				col 
			);
			m_graphics.FillRectangle
			( 
				fPixelRect
				( 
					m_coord.Convert2fPixelPos(umRect.GetStartPoint()), 
					m_graphics.GetClRectSize() 
				), 
				col 
			);
		}
		else
		{
			m_graphics.FillRectangle( m_coord.Convert2fPixelRect( umRect ), col );
		}
	}

	void SetPixelSize( MicroMeter const s ) 
	{
		DrawContext::SetPixelSize( s );
		m_scale.SetHorzPixelSize( s.GetValue() );
	}

	virtual void ShowScale( PixelRectSize const & pixRectSize ) const 
	{
		m_scale.DisplayStaticScale( );
	}

	virtual void DisplayText
	(
		MicroMeterRect      const & umRect,
		std::wstring        const & wstr,
		D2D1::ColorF        const   colF,
		IDWriteTextFormat * const   pTextFormat = nullptr
	) const
	{
		m_graphics.DisplayText( m_coord.Convert2PixelRect( umRect ), wstr, colF, pTextFormat );
	}

private:
	D2D_driver m_graphics;
	Scale      m_scale;
};
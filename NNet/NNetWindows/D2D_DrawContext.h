// D2D_DrawContext.h
//
// NNetSimu

#pragma once

#include "NNetColors.h"
#include "PixelCoordsFp.h"
#include "DrawContext.h"
#include "Direct2D.h"
#include "ShapeId.h"
#include "scale.h"

static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

class D2D_DrawContext: public DrawContext
{
public:
	D2D_DrawContext() {}

	void Start( HWND const hwnd )
	{
		DrawContext::Initialize( );
		m_graphics.Initialize( hwnd );
		m_pScale = new Scale( & m_coord );
		SetStdFontSize( STD_FONT_SIZE );
	}

	void Stop( )
	{
		m_graphics.ShutDown( );
		delete m_pScale;
		m_pScale = nullptr;
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
	}

	bool Zoom( MicroMeter const newSize )
	{
		bool bRes { m_coord.Zoom( newSize ) };
		return bRes;
	}

	void Center( MicroMeterPoint const umPnt, fPixelPoint const fPntPix )
	{
		m_coord.Center( umPnt, fPntPix );
	}

	bool ZoomKeepCrsrPos( PixelPoint const & pixPntCenter, MicroMeter const newSize )
	{
		fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };                   // compute center
		MicroMeterPoint const umPointcenter   { m_coord.Convert2MicroMeterPointPos( fPixPointCenter ) }; // ** BEFORE ** zooming!
		bool bRes { Zoom( newSize ) };
		if ( bRes )
		{
			m_coord.Center( umPointcenter, fPixPointCenter ); 
		}
		else
		{
			MessageBeep( MB_ICONWARNING );
		}
		return bRes;
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
		fPIXEL      const fPixWidth  { m_coord.Convert2fPixel( umWidth ) };
		fPixelPoint const fStartPoint{ m_coord.Convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { m_coord.Convert2fPixelPos( umEndPoint   ) };
		m_graphics.DrawLine( fStartPoint, fEndPoint, fPixWidth, m_bNoColors ? NNetColors::COL_BLACK : col );
	}

	virtual void DrawCircle
	(
		MicroMeterPoint const & umCenterPoint,
		MicroMeter      const   umRadius,
		D2D1::ColorF    const   col  
	) const
	{
		fPixelPoint const fPixCenter { m_coord.Convert2fPixelPos( umCenterPoint ) };
		fPIXEL      const fPixRadius { m_coord.Convert2fPixel   ( umRadius ) };
		m_graphics.DrawCircle( fPixCenter, fPixRadius, m_bNoColors ? NNetColors::COL_BLACK : col );
	}

	virtual void DrawArrow
	( 
		MicroMeterPoint const & umPos, 
		MicroMeterPoint const & umVector, 
		MicroMeter      const   umSize, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		m_graphics.DrawArrow
		( 
			m_coord.Convert2fPixelPos ( umPos ),
			m_coord.Convert2fPixelSize( umVector ), 
			m_coord.Convert2fPixel    ( umSize ),
			m_coord.Convert2fPixel    ( umWidth ),
			m_bNoColors ? NNetColors::COL_BLACK : col 
		);
	}

	virtual void DrawTranspRect( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
	{
		m_graphics.DrawTranspRect( m_coord.Convert2fPixelRect( umRect ), col );
	}

	virtual void ShowScale( PIXEL pixHeight ) const 
	{
		m_pScale->ShowScale( m_graphics, Convert2fPIXEL( pixHeight ) );
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

	void SetNoColors( bool const bMode ) { m_bNoColors = bMode; }

private:
	bool       m_bNoColors { false };
	D2D_driver m_graphics;
	Scale    * m_pScale { nullptr };
};
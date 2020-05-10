// DrawContext.h
//
// NNetModel

#pragma once

#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "ShapeId.h"
#include "scale.h"

class DrawContext
{
public:
	DrawContext() {}

	void Start( HWND const hwnd )
	{
		m_graphics.Initialize( hwnd );
		m_pScale = new Scale( & m_coord );
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

	MicroMeter  GetPixelSize  ( ) const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset( ) const { return m_coord.GetPixelOffset(); }
	void        SetPixelSize  ( MicroMeter  const s ) { m_coord.SetPixelSize  ( s ); }
	void        SetPixelOffset( fPixelPoint const f ) { m_coord.SetPixelOffset( f ); }

	void Center( PixelPoint const & pixPnt )
	{
		fPixelPoint     const fPixPointCenter { convert2fPixelPoint( pixPnt ) };
		MicroMeterPoint const umPointcenter   { m_coord.convert2MicroMeterPointPos( fPixPointCenter ) };
		m_coord.Center( umPointcenter, fPixPointCenter );
	}

	void SetStdFontSize( MicroMeter const & size )
	{
		m_graphics.SetStdFontSize( m_coord.convert2fPixel( size ).GetValue() );
	}

	void DrawLine
	( 
		MicroMeterPoint const & umStartPoint, 
		MicroMeterPoint const & umEndPoint, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		fPIXEL      const fPixWidth  { m_coord.convert2fPixel( umWidth ) };
		fPixelPoint const fStartPoint{ m_coord.convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { m_coord.convert2fPixelPos( umEndPoint   ) };
		m_graphics.DrawLine( fStartPoint, fEndPoint, fPixWidth, col );
	}

	void DrawCircle
	(
		MicroMeterPoint const & umCenterPoint,
		MicroMeter      const   umRadius,
		D2D1::ColorF    const   col  
	) const
	{
		fPixelPoint const fPixCenter { m_coord.convert2fPixelPos( umCenterPoint ) };
		fPIXEL      const fPixRadius { m_coord.convert2fPixel   ( umRadius ) };
		m_graphics.DrawCircle( fPixCenter, fPixRadius, col );
	}

	void DrawArrow
	( 
		MicroMeterPoint const & umPos, 
		MicroMeterPoint const & umVector, 
		MicroMeter      const   umSize, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		fPixelPoint const fPixPos   { m_coord.convert2fPixelPos( umPos ) };
		fPixelPoint const fPixVector{ m_coord.convert2fPixelSize( umVector ) };
		fPIXEL      const fPixSize  { m_coord.convert2fPixel( umSize ) };
		fPIXEL      const fPixWidth { m_coord.convert2fPixel( umWidth ) };
		m_graphics.DrawArrow( fPixPos, fPixVector, fPixSize, fPixWidth, col );
	}

	void DrawTranspRect( PixelRect umRect, D2D1::ColorF col ) const 
	{
		m_graphics.DrawTranspRect( m_coord.convert2fPixelRect( umRect ), col );
	}

	void ShowScale( PIXEL pixHeight ) const 
	{
		m_pScale->ShowScale( m_graphics, convert2fPIXEL( pixHeight ) );
	}

	void DisplayText
	(
		MicroMeterRect      const & umRect,
		std::wstring        const & wstr,
		D2D1::ColorF        const   colF,
		IDWriteTextFormat * const   pTextFormat = nullptr
	) const
	{
		m_graphics.DisplayText( m_coord.convert2PixelRect( umRect ), wstr, colF, pTextFormat );
	}

	PixelCoordsFp const & GetCoordC( ) const { return m_coord; }
	PixelCoordsFp       & GetCoord ( )       { return m_coord; }

private:
	PixelCoordsFp m_coord;
	D2D_driver    m_graphics;
	Scale       * m_pScale { nullptr };
};
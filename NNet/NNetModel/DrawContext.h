// DrawContext.h
//
// NNetModel

#pragma once

#include "win32_util.h"
#include "NNetColors.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "ShapeId.h"
#include "scale.h"

static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

class DrawContext
{
public:
	DrawContext() {}

	void Start( HWND const hwnd )
	{
		m_graphics.Initialize( hwnd );
		m_coord.Reset();
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

	MicroMeter  GetPixelSize  ( ) const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset( ) const { return m_coord.GetPixelOffset(); }
	void        SetPixelSize  ( MicroMeter  const s ) { m_coord.SetPixelSize  ( s ); }
	void        SetPixelOffset( fPixelPoint const f ) { m_coord.SetPixelOffset( f ); }
	void        Move          ( PixelPoint  const d ) { m_coord.Move          ( d ); }

	bool ZoomKeepCrsrPos( PixelPoint const & pixPntCenter, MicroMeter const newSize )
	{
		fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };
		MicroMeterPoint const umPointcenter   { m_coord.Convert2MicroMeterPointPos( fPixPointCenter ) };
		bool bRes { m_coord.Zoom( newSize ) };
		if ( bRes )
		{
			m_coord.Center( umPointcenter, fPixPointCenter ); 
			SetStdFontSize( STD_FONT_SIZE );
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

	void DrawLine
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

	void DrawCircle
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

	void DrawArrow
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

	void DrawTranspRect( MicroMeterRect const & umRect, D2D1::ColorF col ) const 
	{
		m_graphics.DrawTranspRect( m_coord.Convert2fPixelRect( umRect ), col );
	}

	void ShowScale( PIXEL pixHeight ) const 
	{
		m_pScale->ShowScale( m_graphics, Convert2fPIXEL( pixHeight ) );
	}

	void DisplayText
	(
		MicroMeterRect      const & umRect,
		std::wstring        const & wstr,
		D2D1::ColorF        const   colF,
		IDWriteTextFormat * const   pTextFormat = nullptr
	) const
	{
		m_graphics.DisplayText( m_coord.Convert2PixelRect( umRect ), wstr, colF, pTextFormat );
	}

	PixelCoordsFp const & GetCoordC( ) const { return m_coord; }
	PixelCoordsFp       & GetCoord ( )       { return m_coord; }

	void SetNoColors( bool const bMode ) { m_bNoColors = bMode; }

private:
	bool          m_bNoColors { false };
	PixelCoordsFp m_coord;
	D2D_driver    m_graphics;
	Scale       * m_pScale { nullptr };
};
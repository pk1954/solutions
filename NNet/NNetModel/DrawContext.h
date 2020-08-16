// DrawContext.h
//
// NNetModel

#pragma once

#include "NNetColors.h"
#include "PixelCoordsFp.h"
#include "scale.h"

class DrawContext
{
public:
	void Initialize( )
	{
		m_coord.Reset();
	}

	virtual void DrawLine
	( 
		MicroMeterPoint const &, 
		MicroMeterPoint const &, 
		MicroMeter      const, 
		D2D1::ColorF    const
	) const = 0;

	virtual void DrawArrow
	( 
		MicroMeterPoint const &, 
		MicroMeterPoint const &, 
		MicroMeter      const, 
		MicroMeter      const, 
		D2D1::ColorF    const
	) const = 0;

	virtual void DrawCircle( MicroMeterPoint const &, MicroMeter const,	D2D1::ColorF const ) const = 0;
	virtual void DrawTranspRect( MicroMeterRect const &, D2D1::ColorF ) const = 0;
	virtual void ShowScale( PIXEL ) const = 0;
	virtual void DisplayText
	(
		MicroMeterRect      const &,
		std::wstring        const &,
		D2D1::ColorF        const,
		IDWriteTextFormat * const = nullptr
	) const = 0;

	PixelCoordsFp const & GetCoordC( ) const { return m_coord; }

	MicroMeter  GetPixelSize  ( ) const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset( ) const { return m_coord.GetPixelOffset(); }

	void SetPixelSize  ( MicroMeter      const s ) { m_coord.SetPixelSize  ( s ); }
	void SetPixelOffset( fPixelPoint     const f ) { m_coord.SetPixelOffset( f ); }
	void Move          ( PixelPoint      const d ) { m_coord.Move          ( d ); }
	void Move          ( MicroMeterPoint const p ) { m_coord.Move          ( p ); }
	bool Zoom          ( MicroMeter      const s ) { return m_coord.Zoom   ( s ); }
	void Center        ( MicroMeterPoint const u, fPixelPoint const f ) { m_coord.Center( u, f ); }

	void SetNoColors( bool const bMode ) { m_bNoColors = bMode; }

protected:
	bool          m_bNoColors { false };
	PixelCoordsFp m_coord     { };
};
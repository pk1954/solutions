// DrawContext.h
//
// NNetModel

#pragma once

#include "NNetColors.h"
#include "PixelCoordsFp.h"

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

	virtual void FillArrow
	( 
		MicroMeterPoint const &, 
		MicroMeterPoint const &, 
		MicroMeter      const, 
		MicroMeter      const, 
		D2D1::ColorF    const
	) const = 0;

	virtual void DrawCircle        ( MicroMeterCircle  const &, D2D1::ColorF const, MicroMeter const ) const = 0;
	virtual void DrawEllipse       ( MicroMeterEllipse const &, D2D1::ColorF const, MicroMeter const ) const = 0;
	virtual void FillCircle        ( MicroMeterCircle  const &, D2D1::ColorF const ) const = 0;
	virtual void FillEllipse       ( MicroMeterEllipse const &, D2D1::ColorF const ) const = 0;
	virtual void FillRectangle     ( MicroMeterRect    const &, D2D1::ColorF const ) const = 0;
	virtual void FillGradientCircle( MicroMeterCircle  const &, D2D1::ColorF const, D2D1::ColorF const ) const = 0;
	virtual void DrawTranspRect    ( MicroMeterRect    const &, D2D1::ColorF const ) const = 0;
//	virtual void ShowScale         ( PixelRectSize     const &                     ) const = 0;
	virtual void DisplayText
	(
		MicroMeterRect      const &,
		std::wstring        const &,
		D2D1::ColorF        const,
		IDWriteTextFormat * const = nullptr
	) const = 0;

	PixelCoordsFp const & GetCoordC( ) const { return m_coord; }
	PixelCoordsFp       & GetCoord ( )       { return m_coord; }

	MicroMeter  GetPixelSize  ( ) const { return m_coord.GetPixelSize  (); }
	fPixelPoint GetPixelOffset( ) const { return m_coord.GetPixelOffset(); }

	void SetPixelSize  ( MicroMeter      const s ) { m_coord.SetPixelSize  ( s ); }
	void SetPixelOffset( fPixelPoint     const f ) { m_coord.SetPixelOffset( f ); }
	bool Zoom          ( MicroMeter      const s ) { return m_coord.Zoom   ( s ); }
	void Center        ( MicroMeterPoint const u, fPixelPoint const f ) { m_coord.Center( u, f ); }

	template <typename T> void Move(T const d) { m_coord.Move(d); }

	void SetNoColors( bool const bMode ) { m_bNoColors = bMode; }

	bool IsTooSmall( MicroMeterRect const & rect ) const
	{
		MicroMeter const umMin { m_coord.Transform2MicroMeter(10._fPixel) };
		return (rect.GetHeight() < umMin) || (rect.GetWidth() < umMin);
	}

protected:
	bool          m_bNoColors { false };
	PixelCoordsFp m_coord     { };
};
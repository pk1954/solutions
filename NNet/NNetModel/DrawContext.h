// DrawContext.h
//
// NNetModel

#pragma once

#include "win32_NNetWorkThreadInterface.h"

#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "scale.h"

class DrawContext
{
public:
	DrawContext() {}

	MicroMeter  GetPixelSize  ( ) const { return m_pCoord->GetPixelSize  (); }
	fPixelPoint GetPixelOffset( ) const { return m_pCoord->GetPixelOffset(); }
	void        SetPixelSize  ( MicroMeter  const s ) { m_pCoord->SetPixelSize  ( s ); }
	void        SetPixelOffset( fPixelPoint const f ) { m_pCoord->SetPixelOffset( f ); }

	void DrawLine
	( 
		MicroMeterPoint const & umStartPoint, 
		MicroMeterPoint const & umEndPoint, 
		MicroMeter      const   umWidth, 
		D2D1::ColorF    const   col
	) const
	{
		fPIXEL      const fPixWidth  { m_pCoord->convert2fPixel( umWidth ) };
		fPixelPoint const fStartPoint{ m_pCoord->convert2fPixelPos( umStartPoint ) };
		fPixelPoint const fEndPoint  { m_pCoord->convert2fPixelPos( umEndPoint   ) };
		m_pGraphics->DrawLine( fStartPoint, fEndPoint, fPixWidth, col );
	}

	void DrawCircle
	(
		MicroMeterPoint const & umCenterPoint,
		MicroMeter      const   umRadius,
		D2D1::ColorF    const   col  
	) const
	{
		fPixelPoint const fPixCenter { m_pCoord->convert2fPixelPos( umCenterPoint ) };
		fPIXEL      const fPixRadius { m_pCoord->convert2fPixel   ( umRadius ) };
		m_pGraphics->DrawCircle( fPixCenter, fPixRadius, col );
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
		fPixelPoint const fPixPos   { m_pCoord->convert2fPixelPos( umPos ) };
		fPixelPoint const fPixVector{ m_pCoord->convert2fPixelSize( umVector ) };
		fPIXEL      const fPixSize  { m_pCoord->convert2fPixel( umSize ) };
		fPIXEL      const fPixWidth { m_pCoord->convert2fPixel( umWidth ) };
		m_pGraphics->DrawArrow( fPixPos, fPixVector, fPixSize, fPixWidth, col );
	}

	void DrawTranspRect( PixelRect umRect, D2D1::ColorF col ) const 
	{
		m_pGraphics->DrawTranspRect( m_pCoord->convert2fPixelRect( umRect ), col );
	}

	void ShowScale( PIXEL pixHeight ) const 
	{
		m_pScale->ShowScale( * m_pGraphics, convert2fPIXEL( pixHeight ) );
	}

/////////////////////////////////////////////////////////////////////

	void PostSetPulseRate( ShapeId const id, fHertz const fNewValue )
	{
		m_pNNetWorkThread->PostThreadMsg( static_cast<UINT>( NNetWorkThreadMessage::Id::PULSE_RATE ), id.GetValue(), (LPARAM &)fNewValue );
	}



private:
	NNetWorkThreadInterface * m_pNNetWorkThread;

	PixelCoordsFp * m_pCoord;
	D2D_driver    * m_pGraphics;
	Scale         * m_pScale;
};
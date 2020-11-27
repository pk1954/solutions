// Measurement.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include <iomanip>
#include "Direct2D.h"
#include "Measurement.h"

using std::wostringstream;
using std::setprecision;
using std::fixed;
using std::endl;

static D2D1::ColorF const COL_STRONG { 0.0f, 0.4f, 0.0f, 1.0f };
static D2D1::ColorF const COL_WEAK   { 0.0f, 1.0f, 0.0f, 0.3f };

void Measurement::Initialize( D2D_driver * const pGraphics )
{
	m_pGraphics   = pGraphics;
	m_pTextFormat = pGraphics->NewTextFormat( 12.f );
}

void Measurement::ResetLimits( )
{
	if ( m_fPixClientWidth.IsNotNull() )
	{
		m_fPixLeftLimit  = m_fPixClientWidth * 0.1f;
		m_fPixRightLimit = m_fPixClientWidth * 0.9f;
	}
}

void Measurement::SetClientRectSize( PIXEL const width, PIXEL const height )
{
	m_fPixClientWidth  = Convert2fPIXEL( width );
	m_fPixClientHeight = Convert2fPIXEL( height );
	ResetLimits( );
}

bool Measurement::Select( fPIXEL const fPix )
{
	if ( m_bActive )
	{
		m_bSelectedLeft  = isCloseTo( fPix, m_fPixLeftLimit );
		m_bSelectedRight = isCloseTo( fPix, m_fPixRightLimit );
		if ( m_bSelectedLeft || m_bSelectedRight )
			return true;
	}
	return false;
}

void Measurement::MoveSelection( fPIXEL const fPix )
{
	if ( m_bActive )
	{
		if ( m_bSelectedLeft )
			m_fPixLeftLimit = fPix;
		else if ( m_bSelectedRight )
			m_fPixRightLimit = fPix;
		m_bLimitsMoved = true;
	}
}

void Measurement::verticalLine( fPIXEL const fPixPosX ) const
{
	m_pGraphics->DrawLine
	( 
		fPixelPoint( fPixPosX, 0._fPIXEL ), 
		fPixelPoint( fPixPosX, m_fPixClientHeight ), 
		1._fPIXEL, 
		RGB( 0, 0, 0 )  
	);
}

void Measurement::emphasizedLine( fPIXEL const fPixPosX ) const
{
	fPixelRect const rect1 
	{ 
		fPixPosX - GRADIENT_WIDTH, 
		0._fPIXEL, 
		fPixPosX, 
		m_fPixClientHeight 
	};

	fPixelRect const rect2 
	{ 
		rect1.GetRight(), 
		0._fPIXEL, 
		fPixPosX + GRADIENT_WIDTH, 
		m_fPixClientHeight 
	};

	m_pGraphics->DrawGradientRect( rect1, COL_WEAK,   COL_STRONG );
	m_pGraphics->DrawGradientRect( rect2, COL_STRONG, COL_WEAK   );
}

bool const Measurement::isCloseTo( fPIXEL const fPix1, fPIXEL const fPix2 ) const
{
	return fabs(fPix1.GetValue() - fPix2.GetValue() ) <= GRADIENT_WIDTH.GetValue();
}

void Measurement::DisplayDynamicScale( fMicroSecs const fMicroSecsPerPixel ) const
{
	if ( m_bActive )
	{
		if ( m_bSelectedLeft )
			emphasizedLine( m_fPixLeftLimit );
		verticalLine( m_fPixLeftLimit );

		fPixelRect const rect
		{ 
			m_fPixLeftLimit + (m_bSelectedLeft ? GRADIENT_WIDTH : 0.0_fPIXEL), 
			0._fPIXEL, 
			m_fPixRightLimit - (m_bSelectedRight ? GRADIENT_WIDTH : 0.0_fPIXEL),
			m_fPixClientHeight 
		};
		m_pGraphics->DrawTranspRect( rect, COL_WEAK );

		if ( m_bSelectedRight )
			emphasizedLine( m_fPixRightLimit );
		verticalLine( m_fPixRightLimit );

		static COLORREF const COLOR        { RGB( 0, 0, 0 ) };  // CLR_BLACK
		fPIXEL          const fPixDistance { m_fPixRightLimit - m_fPixLeftLimit };
		fMicroSecs      const usMeasured   { fMicroSecsPerPixel * fPixDistance.GetValue() };
		fHertz          const frequency    { Frequency( usMeasured) };

		PIXEL posRight { Convert2PIXEL(m_fPixRightLimit - GRADIENT_WIDTH) };
		PIXEL posTop   { Convert2PIXEL(GRADIENT_WIDTH) };

		PixelRect pixRect
		( 
			posRight - 50_PIXEL, // left
			posTop,              // top
			posRight,            // right
			posTop + 35_PIXEL    // bottom
		);

		wostringstream wBuffer;
		wstring        wstrTime;
		Format2wstring( usMeasured, wstrTime );
		wBuffer << wstrTime << endl;
		wBuffer << fixed << setprecision(2);
		wBuffer << frequency << L" Hz";

		static D2D1::ColorF const COL_BACKGROUND { 1.0f, 1.0f, 1.0f, 1.0f };

		m_pGraphics->DrawTranspRect( Convert2fPixelRect(pixRect), COL_BACKGROUND );
		m_pGraphics->DisplayText   ( pixRect, wBuffer.str( ), COLOR, m_pTextFormat );
	}
}

// scale.cpp
//
// NNetModel

#include "stdafx.h"
#include <sstream> 
#include "MoreTypes.h"
#include "Direct2D.h"
#include "scale.h"

using std::wostringstream;

void Scale::Initialize
( 
	D2D_driver * const   pGraphics, 
	wstring      const & wstrLogUnit
)
{
	m_pGraphics   = pGraphics;
	m_pTextFormat = pGraphics->NewTextFormat( 12.f );
	m_wstrLogUnit = wstrLogUnit;
}

void Scale::SetClientRectSize( PIXEL const width, PIXEL const height )
{
	m_fPixClientWidth  = Convert2fPixel( width );
	m_fPixClientHeight = Convert2fPixel( height );
	m_fPixVertPos      = m_fPixClientHeight - 20._fPixel;
	calcScaleParams();
}

void Scale::SetHorzPixelSize( float const fSize ) 
{ 
	m_fHorzPixelSize = fSize; 
	calcScaleParams();
};

void Scale::calcScaleParams( )
{
	LogUnits logLengthMax    { m_fPixClientWidth.GetValue() * m_fHorzPixelSize * 0.9f };
	float    fFractPart      { modff( log10f( logLengthMax ), & m_fIntegerPart ) };
	LogUnits logLength       { LogUnits( powf( 10.0, m_fIntegerPart ) ) };
	float    fFirstDigit     { (fFractPart >= log10f(5.f)) ? 5.f : (fFractPart >= log10f(2.f)) ? 2.f : 1.f };
	fPixel   fPixScaleLength { fPixel(fFirstDigit * logLength / m_fHorzPixelSize) };
	fPixel   fPixHorzOffset  { (m_fPixClientWidth - fPixScaleLength) / 2 };
	m_iFirstDigit  = static_cast<int>(fFirstDigit);
	m_fPixPntStart = fPixelPoint( fPixHorzOffset,                   m_fPixVertPos );
	m_fPixPntEnd   = fPixelPoint( fPixHorzOffset + fPixScaleLength, m_fPixVertPos );
}

void Scale::DisplayStaticScale( ) const
{
	m_pGraphics->DrawLine( m_fPixPntStart, m_fPixPntEnd, 1._fPixel, SCALE_COLOR );
	displayTicks    ( m_fPixPntStart, m_fPixPntEnd, m_fIntegerPart, m_iFirstDigit );
	displayScaleText( m_fPixPntEnd, m_fIntegerPart );
}

void Scale::displayTicks
( 
	fPixelPoint const fPixPoint1, 
	fPixelPoint const fPixPoint2, 
	float       const fLog10, 
	int         const iFirstDigit 
) const
{
	fPixelPoint fLongTick  ( 0._fPixel, 10._fPixel );
	fPixelPoint fMiddleTick( 0._fPixel,  7._fPixel );
	fPixelPoint fSmallTick ( 0._fPixel,  5._fPixel );

	fPixelPoint fTickPos( fPixPoint1 );
	fPixelPoint fTickDist( (fPixPoint2.GetX() - fPixPoint1.GetX()) / 10, 0._fPixel );

	m_pGraphics->DrawLine( fPixPoint1 - fLongTick, fTickPos, 1._fPixel, SCALE_COLOR );

	displayScaleNumber( fTickPos, fLog10, 0 );

	if ( iFirstDigit == 1 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPixel, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPixel, SCALE_COLOR );
		displayScaleNumber( fTickPos, fLog10 - 1.0f, 5 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPixel, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 2 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPixel, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPixel, SCALE_COLOR );
		displayScaleNumber( fTickPos, fLog10, 1 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPixel, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 5 )
	{
		for ( int i = 0;; )
		{
			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPixel, SCALE_COLOR );

			if ( ++i > 4 )
				break;

			fTickPos += fTickDist;
			m_pGraphics->DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPixel, SCALE_COLOR );
			displayScaleNumber( fTickPos, fLog10, i );
		}
	}
	else 
		assert( false );

	displayScaleNumber( fPixPoint2, fLog10, iFirstDigit );
	m_pGraphics->DrawLine( fPixPoint2 - fLongTick, fPixPoint2, 1._fPixel, SCALE_COLOR );
}

void Scale::displayScaleNumber
( 
	fPixelPoint const fPos, 
	float       const fLog10, 
	int         const iFirstDigit 
) const
{
	static PIXEL const textWidth  { 40_PIXEL };
	static PIXEL const textHeight { 20_PIXEL };
	static PIXEL const horzDist   {  2_PIXEL };
	static PIXEL const vertDist   { 12_PIXEL };

	PixelPoint const pixPos { Convert2PixelPoint( fPos ) };
	PixelRect  const pixRect
	{ 
		pixPos.GetX() + horzDist - textWidth,  // left
		pixPos.GetY() - vertDist - textHeight, // top
		pixPos.GetX() + horzDist + textWidth,  // right
		pixPos.GetY() - vertDist               // bottom
	};

	wostringstream wBuffer;
	wBuffer << iFirstDigit;

	if ( iFirstDigit > 0 )
	{
		int iLog10  = static_cast<int>( floor(fLog10) );
		int nDigits = iLog10 % 3;

		while ( nDigits-- )
			wBuffer << L"0";
	}

	m_pGraphics->DisplayText( pixRect, wBuffer.str( ), SCALE_COLOR, m_pTextFormat );
}

void Scale::displayScaleText
( 
	fPixelPoint const fPos, 
	float       const fLog10
) const
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   =  0_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL posX { Convert2PIXEL(fPos.GetX()) };
	PIXEL posY { Convert2PIXEL(fPos.GetY()) };

	PixelRect pixRect
	( 
		posX + horzDist,              // left
		posY - vertDist - textHeight, // top
		posX + horzDist + textWidth,  // right
		posY - vertDist               // bottom
	);

	wostringstream wBuffer;

	if ( fLog10 < 3 )
		wBuffer << L"\u03BC";
	else if ( fLog10 < 6 )
		wBuffer << L"m";
	wBuffer << m_wstrLogUnit;

	m_pGraphics->DisplayText( pixRect, wBuffer.str( ), SCALE_COLOR, m_pTextFormat );
}

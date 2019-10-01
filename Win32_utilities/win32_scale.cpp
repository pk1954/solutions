// win32_scale.cpp
//
// Utilities

#include "stdafx.h"
#include "MoreTypes.h"
#include "fPixelCoords.h"
#include "win32_graphicsInterface.h"
#include "win32_scale.h"

Scale::Scale( GraphicsInterface * const pGraphics, fPixelCoords * const pCoords )
 : m_pGraphics( pGraphics ),
   m_pfPixelCoords( pCoords )
{ }

void Scale::ShowScale (fPIXEL const height )
{
	m_pGraphics->SetFontSize( 12_PIXEL );

	fPIXEL const vertPos    = height - 20._fPIXEL;
	fPIXEL const horzPos    = 100._fPIXEL;
	fPIXEL const lengthMax  = 500._fPIXEL;

	double           dIntegerPart;
	MicroMeter const umLengthExact = m_pfPixelCoords->fPixel2MicroMeter( lengthMax );
	double     const logValue      = log10( umLengthExact.GetValue() );
	double     const fractPart     = modf( logValue, & dIntegerPart );
	double     const nextPowerOf10 = pow( 10.0, dIntegerPart );
	MicroMeter const umLength      = MicroMeter( nextPowerOf10 );

	int iFirstDigit = ( fractPart >= log10( 5 ) ) 
		? 5
		: ( fractPart >= log10( 2 ) )
		? 2
		: 1;

	fPIXEL      const fPixLength = m_pfPixelCoords->MicroMeter2fPixel( umLength * iFirstDigit );
	fPixelPoint const fPixPoint1( horzPos, vertPos );
	fPixelPoint const fPixPoint2( horzPos + fPixLength, vertPos );

	m_pGraphics->AddfPixelLine( fPixPoint1, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
	displayTicks( fPixPoint1, fPixPoint2, dIntegerPart, iFirstDigit );
	displayScaleText( fPixPoint2, dIntegerPart );
}

void Scale::displayTicks( fPixelPoint const fPixPoint1, fPixelPoint const fPixPoint2, double const dLog10, int const iFirstDigit )
{
	fPixelPoint fLongTick  (  0._fPIXEL, 10._fPIXEL );
	fPixelPoint fMiddleTick(  0._fPIXEL,  7._fPIXEL );
	fPixelPoint fSmallTick (  0._fPIXEL,  5._fPIXEL );

	fPixelPoint fTickPos( fPixPoint1 );
	fPixelPoint fTickDist( (fPixPoint2.GetX() - fPixPoint1.GetX()) / 10, 0._fPIXEL );

	m_pGraphics->AddfPixelLine( fPixPoint1 - fLongTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

	displayScaleNumber( fTickPos, dLog10, 0 );

	if ( iFirstDigit == 1 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( fTickPos, dLog10 - 1.0, 5 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 2 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( fTickPos, dLog10, 1 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 5 )
	{
		for ( int i = 0;; )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

			if ( ++i > 4 )
				break;

			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
			displayScaleNumber( fTickPos, dLog10, i );
		}
	}
	else 
		assert( false );

	displayScaleNumber( fPixPoint2, dLog10, iFirstDigit );
	m_pGraphics->AddfPixelLine( fPixPoint2 - fLongTick, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
}

void Scale::displayScaleNumber( fPixelPoint const fPos, double const dLog10, int const iFirstDigit )
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   =  2_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL posX = PIXEL(static_cast<long>(fPos.GetXvalue()));
	PIXEL posY = PIXEL(static_cast<long>(fPos.GetYvalue()));

	PixelRect pixRect
	( 
		posX + horzDist - textWidth,  // left
		posY - vertDist - textHeight, // top
		posX + horzDist + textWidth,  // right
		posY - vertDist               // bottom
	);

	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();

	m_wBuffer << iFirstDigit;

	if ( iFirstDigit > 0 )
	{
		int iLog10  = static_cast<int>( floor(dLog10) );
		int nDigits = iLog10 % 3;

		while ( nDigits-- )
			m_wBuffer << L"0";
	}

	m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_CENTER, SCALE_COLOR );
}

void Scale::displayScaleText( fPixelPoint const fPos, double const dLog10 )
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   = 16_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL posX = PIXEL(static_cast<long>(fPos.GetXvalue()));
	PIXEL posY = PIXEL(static_cast<long>(fPos.GetYvalue()));

	PixelRect pixRect
	( 
		posX + horzDist,              // left
		posY - vertDist - textHeight, // top
		posX + horzDist + textWidth,  // right
		posY - vertDist               // bottom
	);

	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();

	if ( dLog10 < 3 )
		m_wBuffer << L"\u03BCm";
	else if ( dLog10 < 6 )
		m_wBuffer << L"mm";
	else
		m_wBuffer << L"m";

	m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_LEFT, SCALE_COLOR );
}

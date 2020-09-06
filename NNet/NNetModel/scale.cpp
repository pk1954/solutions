// scale.cpp
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "Direct2D.h"
#include "scale.h"

using LogUnits = float;

IDWriteTextFormat * Scale::m_pTextFormat;

wostringstream Scale::m_wBuffer;

void Scale::Display
( 
	D2D_driver    const & graphics, 
	PixelRectSize const & pixRectSize,
	float         const   fHorzPixelSize,
	wstring       const & wstrLogUnit 
)
{
	fPixelRectSize fPixSize { Convert2fPixelRectSize( pixRectSize ) };

	if ( ! m_pTextFormat )
		m_pTextFormat = graphics.NewTextFormat( 12.f );

	fPixelPoint const fPixOffset { 100._fPIXEL, fPixSize.GetY() - 20._fPIXEL };

	fPixSize -= fPixelRectSize( fPixOffset.GetX(), 0._fPIXEL );

	float          fIntegerPart;
	LogUnits const umLengthMax = fPixSize.GetXvalue() * fHorzPixelSize;
	float    const fractPart   = modff( log10f( umLengthMax ), & fIntegerPart );
	LogUnits const logLength   = LogUnits( powf( 10.0, fIntegerPart ) );

	int iFirstDigit = ( fractPart >= log10f( 5.0f ) ) 
		? 5
		: ( fractPart >= log10f( 2.0f ) )
	    	? 2
		    : 1;

	fPIXEL      const fPixScaleLength = fPIXEL(static_cast<float>(iFirstDigit) * logLength / fHorzPixelSize);
	fPixelPoint const fPixPoint1( fPixOffset );
	fPixelPoint const fPixPoint2( fPixOffset + fPixelPoint( fPixScaleLength, 0._fPIXEL ) );

	graphics.DrawLine( fPixPoint1, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
	displayTicks    ( graphics, fPixPoint1, fPixPoint2, fIntegerPart, iFirstDigit );
	displayScaleText( graphics, fPixPoint2, fIntegerPart, wstrLogUnit );
}

void Scale::displayTicks( D2D_driver const & graphics, fPixelPoint const fPixPoint1, fPixelPoint const fPixPoint2, float const fLog10, int const iFirstDigit )
{
	fPixelPoint fLongTick  ( 0._fPIXEL, 10._fPIXEL );
	fPixelPoint fMiddleTick( 0._fPIXEL,  7._fPIXEL );
	fPixelPoint fSmallTick ( 0._fPIXEL,  5._fPIXEL );

	fPixelPoint fTickPos( fPixPoint1 );
	fPixelPoint fTickDist( (fPixPoint2.GetX() - fPixPoint1.GetX()) / 10, 0._fPIXEL );

	graphics.DrawLine( fPixPoint1 - fLongTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

	displayScaleNumber( graphics, fTickPos, fLog10, 0 );

	if ( iFirstDigit == 1 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		graphics.DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( graphics, fTickPos, fLog10 - 1.0f, 5 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 2 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		graphics.DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( graphics, fTickPos, fLog10, 1 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 5 )
	{
		for ( int i = 0;; )
		{
			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

			if ( ++i > 4 )
				break;

			fTickPos += fTickDist;
			graphics.DrawLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
			displayScaleNumber( graphics, fTickPos, fLog10, i );
		}
	}
	else 
		assert( false );

	displayScaleNumber( graphics, fPixPoint2, fLog10, iFirstDigit );
	graphics.DrawLine( fPixPoint2 - fLongTick, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
}

void Scale::displayScaleNumber( D2D_driver const & graphics, fPixelPoint const fPos, float const fLog10, int const iFirstDigit )
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

	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();

	m_wBuffer << iFirstDigit;

	if ( iFirstDigit > 0 )
	{
		int iLog10  = static_cast<int>( floor(fLog10) );
		int nDigits = iLog10 % 3;

		while ( nDigits-- )
			m_wBuffer << L"0";
	}

	graphics.DisplayText( pixRect, m_wBuffer.str( ), SCALE_COLOR, m_pTextFormat );
}

void Scale::displayScaleText
( 
	D2D_driver  const & graphics, 
	fPixelPoint const   fPos, 
	float       const   fLog10, 
	wstring     const & wstrLogUnit 
)
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

	if ( fLog10 < 3 )
		m_wBuffer << L"\u03BC";
	else if ( fLog10 < 6 )
		m_wBuffer << L"m";
	m_wBuffer << wstrLogUnit;

	graphics.DisplayText( pixRect, m_wBuffer.str( ), SCALE_COLOR, m_pTextFormat );
}

// scale.h
//
// NNetModel

#pragma once

#include <string> 
#include "PixelTypes.h"

using std::wstring;

class D2D_driver;

struct IDWriteTextFormat;

class Scale
{
public:
	void Initialize( D2D_driver * const, wstring const & );

	void SetClientRectSize( PIXEL const, PIXEL const );
	void SetHorzPixelSize( float const );

	void DisplayStaticScale( ) const;
private:
	using LogUnits = float;

	inline static COLORREF const SCALE_COLOR { RGB( 0, 0, 0 ) };  // CLR_BLACK

	fPIXEL m_fPixClientWidth { 0.0_fPIXEL };
	fPIXEL m_fPixClientHeight{ 0.0_fPIXEL };
	fPIXEL m_fPixVertPos     { 0.0_fPIXEL };

	D2D_driver        * m_pGraphics; 
	IDWriteTextFormat * m_pTextFormat;
	wstring             m_wstrLogUnit;
	float               m_fHorzPixelSize { 1.0f };

	// parameters for fixed scale

	float       m_fIntegerPart { 0.0f };
	int         m_iFirstDigit  { 0 };    
	fPixelPoint m_fPixPntStart { fPP_NULL };
	fPixelPoint m_fPixPntEnd   { fPP_NULL };

	// private functions

	void displayTicks      ( fPixelPoint const, fPixelPoint const, float const, int const ) const;
	void displayScaleNumber( fPixelPoint const, float const, int const ) const;
	void displayScaleText  ( fPixelPoint const, float const ) const;
	void calcScaleParams();
};

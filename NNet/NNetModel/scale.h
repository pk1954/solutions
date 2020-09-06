// scale.h
//
// NNetModel

#pragma once

#include <string> 
#include <sstream> 
#include "PixelTypes.h"

using std::wstring;
using std::wostringstream;

class D2D_driver;

struct IDWriteTextFormat;

class Scale
{
public:
	static void Display
	( 
		D2D_driver    const &, 
		PixelRectSize const &, 
		float         const, 
		wstring       const & 
	);

private:
	static COLORREF const SCALE_COLOR { RGB( 0, 0, 0 ) };  // CLR_BLACK

	static wostringstream m_wBuffer;

	static IDWriteTextFormat * m_pTextFormat;

	static void displayTicks      ( D2D_driver const &, fPixelPoint const, fPixelPoint const, float const, int const );
	static void displayScaleNumber( D2D_driver const &, fPixelPoint const, float const, int const );
	static void displayScaleText  ( D2D_driver const &, fPixelPoint const, float const, wstring const & );
};

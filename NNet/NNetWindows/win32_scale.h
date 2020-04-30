// win32_scale.h
//
// NNetWindows

#pragma once

#include <sstream> 
#include "PixelTypes.h"

class D2D_driver;
class PixelCoordsFp;

struct IDWriteTextFormat;

class Scale
{
public:
	Scale( D2D_driver *, PixelCoordsFp * const );

	~Scale( );

	void ShowScale( D2D_driver const *, fPIXEL const );

private:
	static COLORREF const SCALE_COLOR { RGB( 0, 0, 0 ) };  // CLR_BLACK

	std::wostringstream m_wBuffer;

	PixelCoordsFp * m_pfPixelCoords { nullptr };

	static IDWriteTextFormat * m_pTextFormat;

	void displayTicks      ( D2D_driver const *, fPixelPoint const, fPixelPoint const, float const, int const );
	void displayScaleNumber( D2D_driver const *, fPixelPoint const, float const, int const );
	void displayScaleText  ( D2D_driver const *, fPixelPoint const, float const );
};

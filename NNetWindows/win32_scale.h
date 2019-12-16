// win32_scale.h
//
// Utilities

#pragma once

#include <sstream> 
#include "PixelTypes.h"

class D2D_driver;
class PixelCoordsFp;

class Scale
{
public:
	Scale( D2D_driver * const,  PixelCoordsFp * const );

	void ShowScale( fPIXEL const );

private:
	static COLORREF const SCALE_COLOR = RGB( 0, 0, 0 );  // CLR_BLACK

	std::wostringstream m_wBuffer;

	D2D_driver    * m_pGraphics;
	PixelCoordsFp * m_pfPixelCoords;

	void displayTicks( fPixelPoint const, fPixelPoint const, float const, int const );
	void displayScaleNumber( fPixelPoint const, float const, int const );
	void displayScaleText( fPixelPoint const, float const );
};

// win32_scale.h
//
// Utilities

#pragma once

#include <sstream> 
#include "PixelTypes.h"

class GraphicsInterface;
class PixelCoordsFp;

class Scale
{
public:
	Scale( GraphicsInterface * const,  PixelCoordsFp * const );

	void ShowScale( fPIXEL const );

private:
	static COLORREF const SCALE_COLOR = RGB( 0, 0, 0 );  // CLR_BLACK

	std::wostringstream m_wBuffer;

	GraphicsInterface * m_pGraphics;
	PixelCoordsFp      * m_pfPixelCoords;

	void displayTicks( fPixelPoint const, fPixelPoint const, double const, int const );
	void displayScaleNumber( fPixelPoint const, double const, int const );
	void displayScaleText( fPixelPoint const, double const );
};

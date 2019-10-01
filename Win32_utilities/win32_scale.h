// win32_scale.h
//
// Utilities

#pragma once

#include <sstream> 
#include "PixelTypes.h"

class GraphicsInterface;
class fPixelCoords;

class Scale
{
public:
	Scale( GraphicsInterface * const,  fPixelCoords * const );

	void ShowScale( fPIXEL const );

private:
	static COLORREF const SCALE_COLOR = RGB( 0, 0, 0 );  // CLR_BLACK

	std::wostringstream m_wBuffer;

	GraphicsInterface * m_pGraphics;
	fPixelCoords      * m_pfPixelCoords;

	void displayTicks( fPixelPoint const, fPixelPoint const, double const, int const );
	void displayScaleNumber( fPixelPoint const, double const, int const );
	void displayScaleText( fPixelPoint const, double const );
};

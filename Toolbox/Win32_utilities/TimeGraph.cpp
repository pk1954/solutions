// TimeGraph.cpp 
//
// Win32_utilities

module;

#include <Windows.h>

module TimeGraph;

import std.compat;
import Debug;
import Types;
import Scale;
import PixFpDimension;
import GraphicsWindow;

using std::round;

TimeGraph::TimeGraph
(
	HWND    const hwndParent,
	LPCTSTR const szClass
)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		szClass, 
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	SetDefaultBackgroundColor();
};

fMicroSecs TimeGraph::GetTime(fPixelPoint const & p) const 
{ 
	return GetTime(p.GetX()); 
}

fMicroSecs TimeGraph::GetTime(fPixel const fPixX) const 
{ 
	Assert(m_pHorzCoord);
	return Pixel2scaleTime(fPixX);
}

fPixel TimeGraph::xTime(fMicroSecs const time) const 
{ 
	Assert(m_pHorzCoord);
	return Scale2pixelTime(time);
}

bool TimeGraph::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width) - m_fPixRightBorder;
	m_fPixBottom = Convert2fPixel(height);
	return true;
}

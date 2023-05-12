// TimeGraph.cpp 
//
// Win32_utilities

module;

#include <cassert>
#include <Windows.h>

module TimeGraph;

import Types;
import PixFpDimension;
import GraphicsWindow;

TimeGraph::TimeGraph
(
	HWND                   const hwndParent,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  : m_pHorzCoord(pHorzCoord)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassTimeGraph", 
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	assert(m_pHorzCoord);
	m_pHorzCoord->RegisterObserver(*this); 
};

TimeGraph::~TimeGraph()
{
	m_pHorzCoord->UnregisterObserver(*this); 
}

fMicroSecs TimeGraph::getTime(fPixelPoint const & p) const 
{ 
	return getTime(p.GetX()); 
}

fMicroSecs TimeGraph::getTime(fPixel const fPixX) const 
{ 
	return m_pHorzCoord->Transform2logUnitPos(fPixX); 
}

fPixel TimeGraph::xTime(fMicroSecs const time) const 
{ 
	return fPixel(m_pHorzCoord->Transform2fPixelPos(time)); 
}

bool TimeGraph::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width) - m_fPixRightBorder;
	m_fPixBottom = Convert2fPixel(height);
	return true;
}

// TimeGraph.cpp 
//
// Win32_utilities

module;

#include <cmath>
#include <cassert>
#include <Windows.h>

module TimeGraph;

import Types;
import Scale;
import PixFpDimension;
import GraphicsWindow;

using std::round;

TimeGraph::TimeGraph(HWND const hwndParent)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassTimeGraph", 
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	SetDefaultBackgroundColor();
};

void TimeGraph::SetHorzCoord(PixFpDimension<fMicroSecs>* pHorzCoord)
{
	assert(!m_pHorzCoord);
	m_pHorzCoord = pHorzCoord;
	assert(m_pHorzCoord);
	m_pHorzCoord->RegisterObserver(*this);
}

TimeGraph::~TimeGraph()
{
	if (m_pHorzCoord)
		m_pHorzCoord->UnregisterObserver(*this);
}

fMicroSecs TimeGraph::GetTime(fPixelPoint const & p) const 
{ 
	return GetTime(p.GetX()); 
}

fMicroSecs TimeGraph::GetTime(fPixel const fPixX) const 
{ 
	assert(m_pHorzCoord);
	return m_pHorzCoord->Transform2logUnitPos(fPixX);
}

fPixel TimeGraph::xTime(fMicroSecs const time) const 
{ 
	assert(m_pHorzCoord);
	return m_pHorzCoord->Transform2fPixelPos(time);
}

bool TimeGraph::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width) - m_fPixRightBorder;
	m_fPixBottom = Convert2fPixel(height);
	return true;
}

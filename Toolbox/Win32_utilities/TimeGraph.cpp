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

void TimeGraph::SetHorzScale(Scale<fMicroSecs>* pHorzScale)
{
	m_pHorzScale = pHorzScale;
	assert(m_pHorzScale);
	SetHorzCoord(&m_pHorzScale->GetDimension());
}

void TimeGraph::SetDefaultBackgroundColor()
{
	m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
}

TimeGraph::~TimeGraph()
{
	if (m_pHorzCoord)
		m_pHorzCoord->UnregisterObserver(*this);
}

fMicroSecs TimeGraph::getTime(fPixelPoint const & p) const 
{ 
	return getTime(p.GetX()); 
}

fMicroSecs TimeGraph::getTime(fPixel const fPixX) const 
{ 
	assert(m_pHorzCoord);
	fMicroSecs fRes { m_pHorzCoord->Transform2logUnitPos(fPixX) };
	if (m_pHorzScale)
	{
		fMicroSecs const fRaster { m_pHorzScale->GetRaster() };
		fRes = fRaster * round(fRes / fRaster);
	}
	return fRes;
}

fPixel TimeGraph::xTime(fMicroSecs const time) const 
{ 
	assert(m_pHorzCoord);
	return fPixel(m_pHorzCoord->Transform2fPixelPos(time));
}

bool TimeGraph::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width) - m_fPixRightBorder;
	m_fPixBottom = Convert2fPixel(height);
	return true;
}

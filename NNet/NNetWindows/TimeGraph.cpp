// TimeGraph.cpp 
//
// NNetModel

#include "stdafx.h"
#include "TimeGraph.h"

TimeGraph::TimeGraph
(
	HWND                   const hwndParent,
	SignalGenerator            & sigGen,
	PixFpDimension<fMicroSecs> * pHorzCoord
)
  : m_pHorzCoord(pHorzCoord),
	m_sigGen(sigGen)
{
	GraphicsWindow::Initialize
	(
		hwndParent, 
		L"ClassTimeGraph", 
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE
	);
	assert(m_pHorzCoord);
	m_sigGen.Register(*this); // signal generator data can be changed from outside
	m_pHorzCoord->RegisterObserver(*this); 
};

TimeGraph::~TimeGraph()
{
	m_pHorzCoord->UnregisterObserver(*this); 
	m_sigGen.Unregister(*this);
}

fMicroSecs TimeGraph::getTime(fPixelPoint const & p) const 
{ 
	return m_pHorzCoord->Transform2logUnitPos(p.GetX()); 
}

fPixel TimeGraph::xTime(fMicroSecs const time) const 
{ 
	return fPixel(m_pHorzCoord->Transform2fPixelPos(time)); 
}

bool TimeGraph::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixRight  = Convert2fPixel(width);
	m_fPixBottom = Convert2fPixel(height);
	return true;
}

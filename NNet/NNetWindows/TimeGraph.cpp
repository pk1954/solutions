// TimeGraph.cpp 
//
// NNetModel

#include "stdafx.h"
#include "TimeGraph.h"

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

void TimeGraph::SetSignalGenerator(SignalGenerator * const pSigGen)
{
	if (m_pSigGen)
		m_pSigGen->Unregister(*this);
	m_pSigGen = pSigGen;
	m_pSigGen->Register(*this); // signal generator data can be changed from outside
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

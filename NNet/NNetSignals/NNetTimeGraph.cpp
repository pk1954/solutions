// NNetTimeGraph.cpp
//
// NNetSignals

module;

#include <d2d1helper.h>
#include <cassert>
#include <compare>
#include <math.h>
#include <Windows.h>

module NNetSignals:NNetTimeGraph;

NNetTimeGraph::NNetTimeGraph
(
	HWND    const hwndParent,
	LPCTSTR const szClass
)
	:TimeGraph(hwndParent, szClass)
{}

void NNetTimeGraph::SetModelInterface(NNetModelWriterInterface* const p)
{
	assert(p);
	if (m_pNMWI)
		GetParams()->UnregisterObserver(*this);
	m_pNMWI = p;
	GetParams()->RegisterObserver(*this);
}

void NNetTimeGraph::SetBackgroundColorRef(COLORREF const c)
{
	GetParentRootWindow()->SetBackgroundColorRef(c);
}

void NNetTimeGraph::SetDefaultBackgroundColor()
{ 
	SetBackgroundColorRef(D2D1::ColorF::Ivory); 
}

void NNetTimeGraph::SetVertScaleFreq(Scale<fHertz> * pScale)
{
	if (m_pVertScaleFreq)
		vertCoordFreq().UnregisterObserver(*this);
	m_pVertScaleFreq = pScale;
	if (m_pVertScaleFreq)
		vertCoordFreq().RegisterObserver(*this);
}

void NNetTimeGraph::SetVertScaleVolt(Scale<mV> * pScale)
{
	if (m_pVertScaleVolt)
		vertCoordVolt().UnregisterObserver(*this);
	m_pVertScaleVolt = pScale;
	if (m_pVertScaleVolt)
		vertCoordVolt().RegisterObserver(*this);
}

void NNetTimeGraph::SetHorzScale(Scale<fMicroSecs>* pScale)
{
	if (m_pHorzScale)
		m_pHorzScale->GetDimension().UnregisterObserver(*this);
	m_pHorzScale = pScale;
	if (m_pHorzScale)
	{
		SetHorzCoord(&m_pHorzScale->GetDimension());
		m_pHorzScale->GetDimension().RegisterObserver(*this);
	}
}

void NNetTimeGraph::PaintFreqCurve(SignalGenerator const * pSigGen)
{
	if (m_pVertScaleFreq)
		TimeGraph::Paint
		(
			[this, pSigGen](fMicroSecs const t){ return pixPntStimulusFreq(pSigGen, t); }, 
			0.0_MicroSecs,
			GetTime(xRight()),
			GetParams()->TimeResolution(),
			m_upGraphics->CreateBrush(GetColor(tColor::FREQ)),
			STD_WIDTH
		);
}

void NNetTimeGraph::PaintVoltCurve(SignalGenerator const * pSigGen)
{
	if (m_pVertScaleVolt)
		TimeGraph::Paint
		(
			[this, pSigGen](fMicroSecs const t) { return pixPntStimulusVolt(pSigGen, t); },
			0.0_MicroSecs,
			GetTime(xRight()),
			GetParams()->TimeResolution(),
			m_upGraphics->CreateBrush(GetColor(tColor::VOLT)),
			STD_WIDTH
		);
}

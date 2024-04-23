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

void NNetTimeGraph::SetDefaultBackgroundColor()
{ 
	SetBackgroundColorRef(D2D1::ColorF::Ivory); 
}

void NNetTimeGraph::PaintFreqCurve(SignalGenerator const * pSigGen)
{
	if (!m_pVertCoordFreq)
		return;
	Paint
	(
		[this, pSigGen](fMicroSecs const t){ return fPPStimFreq(pSigGen, t); },
		max(0.0_MicroSecs, GetTime(0.0_fPixel)),
		GetTime(xRight()),
		GetParams()->TimeResolution(),
		m_upGraphics->CreateBrush(GetColor(tColor::FREQ)),
		STD_WIDTH
	);
}

void NNetTimeGraph::PaintVoltCurve(SignalGenerator const * pSigGen)
{
	if (!m_pVertCoordVolt)
		return;
	Paint
	(
		[this, pSigGen](fMicroSecs const t) { return fPPStimVolt(pSigGen, t); },
		max(0.0_MicroSecs, GetTime(0.0_fPixel)),
		GetTime(xRight()),
		GetParams()->TimeResolution(),
		m_upGraphics->CreateBrush(GetColor(tColor::VOLT)),
		STD_WIDTH
	);
}

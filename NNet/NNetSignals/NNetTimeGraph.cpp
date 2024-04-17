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

void NNetTimeGraph::PaintFreqCurve
(
	SignalGenerator const * pSigGen,
	fMicroSecs      const   umStart
)
{
	if (m_pVertCoordFreq)
		TimeGraph::Paint
		(
			[this, pSigGen](fMicroSecs const t){ return pixPntStimulusFreq(pSigGen, t); }, 
			umStart,
			GetTime(xRight()),
			GetParams()->TimeResolution(),
			m_upGraphics->CreateBrush(GetColor(tColor::FREQ)),
			STD_WIDTH
		);
}

void NNetTimeGraph::PaintVoltCurve
(
	SignalGenerator const * pSigGen,
	fMicroSecs      const   umStart
)
{
	if (m_pVertCoordVolt)
		TimeGraph::Paint
		(
			[this, pSigGen](fMicroSecs const t) { return pixPntStimulusVolt(pSigGen, t); },
			umStart,
			GetTime(xRight()),
			GetParams()->TimeResolution(),
			m_upGraphics->CreateBrush(GetColor(tColor::VOLT)),
			STD_WIDTH
		);
}

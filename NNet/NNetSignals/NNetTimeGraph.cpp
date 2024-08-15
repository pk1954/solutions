// NNetTimeGraph.cpp
//
// NNetSignals

module NNetSignals:NNetTimeGraph;

import std;
import std.compat;
import Debug;

using std::min;
using std::max;

NNetTimeGraph::NNetTimeGraph
(
	HWND    const hwndParent,
	LPCTSTR const szClass
)
	:TimeGraph(hwndParent, szClass)
{}

void NNetTimeGraph::SetModelInterface(NNetModelReaderInterface const * const p)
{
	Assert(p);
	if (m_pNMRI)
		const_cast<NNetParameters *>(GetParams())->UnregisterObserver(*this);
	m_pNMRI = p;
	const_cast<NNetParameters *>(GetParams())->RegisterObserver(*this);
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
		m_upGraphics->CreateBrushHandle(GetColor(tColor::FREQ)),
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
		m_upGraphics->CreateBrushHandle(GetColor(tColor::VOLT)),
		STD_WIDTH
	);
}

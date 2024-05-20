// EventViewer.cpp
//
// NNetViewer

module;

#include <string>
#include <cassert>
#include <Windows.h>

module EventViewer;

import Types;
import PixFpDimension;
import NNetModel;

using std::wstring;

EventViewer::EventViewer
(
	HWND const hwndParent,
	NNetModelReaderInterface const * const pNMRI
)
  : NNetTimeGraph(hwndParent, L"ClassEventViewer")
{
    SetModelInterface(pNMRI);
	SetHorzCoord    (&m_horzCoord);
	SetVertCoordVolt(&m_vertCoordVolt);
	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 500000._MicroSecs);
	adjust(GetWindowSize());
	m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
}

void EventViewer::PaintGraphics()
{
	fMicroSecs usStartScan { scanTime(EventType::startScan) };
	fMicroSecs usStopScan  { scanTime(EventType::stopScan) };
	if (usStartScan.IsNull() || usStopScan.IsNull())
	{
		m_upGraphics->DisplayText(L"No scan information available");
		return;
	}
	fPixelRect rect
	{
		Scale2pixelTime(usStartScan),  // left
		0._fPixel,	                   // top
		Scale2pixelTime(usStopScan),   // right
		GetClientHeight()              // bottom
	};
	m_upGraphics->FillRectangle(rect, D2D1::ColorF::DarkGray);

	bool bStimulus = false;
	m_pNMRI->Apply2allEvents
	(
		EventType::stimulus,
		[this, &bStimulus, usStartScan](StimulusEvent const* pStimEvent)
		{
			SignalGenerator const* pSigGen    { m_pNMRI->GetSigGenC(pStimEvent->GetId()) };
			fMicroSecs      const  usStimulus { pStimEvent->GetTime() };
			fMicroSecs      const  usOffset   { usStimulus - usStartScan };
			m_horzCoord.SetOffset(usOffset, false);
			PaintVoltCurve(pSigGen);
			bStimulus = true;
		}
	);
	if (!bStimulus)
		m_upGraphics->DisplayText(L"No stimulus");
	CreateWindowToolTip(m_pNMRI->GetModelFilePath());
};

void EventViewer::adjust(PixelRectSize const clientSize)
{
	if (m_pNMRI)
	{
		adjustHorz(Convert2fPixel(clientSize.GetX()));
		adjustVert(Convert2fPixel(clientSize.GetY()));
	}
}

void EventViewer::adjustHorz(fPixel const fPixWidth)
{
	 m_horzCoord.Adjust(0._MicroSecs, m_pNMRI->TotalScanTime(), 0._fPixel, fPixWidth);
}

void EventViewer::adjustVert(fPixel const fPixHeight)
{
	mV mvPeakMax { 0._mV };
	m_pNMRI->Apply2allEvents
	(
		EventType::stimulus,
		[this, &mvPeakMax](StimulusEvent const* pStimEvent)
		{
			SignalGenerator const* pSigGen { m_pNMRI->GetSigGenC(pStimEvent->GetId()) };
			mV              const  mvPeak  { pSigGen->Amplitude().Peak() };
			if (mvPeak > mvPeakMax)
				mvPeakMax = mvPeak;
		}
	);
	m_vertCoordVolt.Adjust(-mvPeakMax * 0.1f, mvPeakMax * 1.1f, 0._fPixel, fPixHeight);
}

fMicroSecs EventViewer::scanTime(EventType const t) const
{
	fMicroSecs us { fMicroSecs::NULL_VAL() };
	m_pNMRI->Apply2allEvents(t, [&us](NNetEvent const* e){ us = e->GetTime(); });
	return us;
}

bool EventViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetTimeGraph::OnSize(width, height);
	adjust(PixelRectSize(width, height));
	return true;
}

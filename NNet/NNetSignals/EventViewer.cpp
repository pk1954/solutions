// EventViewer.cpp
//
// NNetSignals

module;

#include <cassert>
#include <Windows.h>

module NNetSignals:EventViewer;

import Types;
import PixFpDimension;
import NNetModel;

EventViewer::EventViewer
(
	HWND const hwndParent
)
  : NNetTimeGraph(hwndParent, L"ClassEventViewer")
{
	SetHorzCoord    (&m_horzCoord);
	SetVertCoordVolt(&m_vertCoordVolt);
	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 500000._MicroSecs);
	adjust(GetWindowSize());
	//SetBackgroundColorRef(D2D1::ColorF::Green);
}

void EventViewer::PaintGraphics()
{
	fMicroSecs usStartScan { scanStartTime() };
	fMicroSecs usStopScan  { scanStopTime() };
	fPixelRect rect
	{
		Scale2pixelTime(usStartScan),  // left
		0._fPixel,	                   // top
		Scale2pixelTime(usStopScan),   // right
		GetClientHeight()              // bottom
	};
	m_upGraphics->FillRectangle(rect, D2D1::ColorF::DarkGray);

	bool bStimulus = false;
	m_pNMWI->Apply2allEvents
	(
		EventType::stimulus,
		[this, &bStimulus, usStartScan](StimulusEvent const* pStimEvent)
		{
			SigGenId        const  sigGenId   { pStimEvent->GetId() };
			SignalGenerator const* pSigGen    { m_pNMWI->GetSigGenC(sigGenId) };
			fMicroSecs      const  usStimulus { pStimEvent->GetTimeStamp() };
			fMicroSecs      const  usOffset   { usStimulus - usStartScan };
			m_horzCoord.SetOffset(usOffset, false);
			PaintVoltCurve(pSigGen);
			bStimulus = true;
		}
	);
	if (!bStimulus)
		m_upGraphics->DisplayText(L"No stimulus");
};

void EventViewer::adjust(PixelRectSize const clientSize)
{
	if (m_pNMWI)
	{
		adjustHorz(Convert2fPixel(clientSize.GetX()));
		adjustVert(Convert2fPixel(clientSize.GetY()));
	}
}

void EventViewer::adjustHorz(fPixel const fPixWidth)
{
	 m_horzCoord.Adjust(0._MicroSecs, m_pNMWI->TotalScanTime(), 0._fPixel, fPixWidth);
}

void EventViewer::adjustVert(fPixel const fPixHeight)
{
	mV mvPeakMax { 0._mV };
	m_pNMWI->Apply2allEvents
	(
		EventType::stimulus,
		[this, &mvPeakMax](StimulusEvent const* pStimEvent)
		{
			SigGenId        const  sigGenId     { pStimEvent->GetId() };
			SignalGenerator const* pSigGen      { m_pNMWI->GetSigGenC(sigGenId) };
			BasePeak<mV>    const& voltBasePeak { pSigGen->Amplitude() };
			mV              const  mvPeak       { voltBasePeak.Peak() };
			if (mvPeak > mvPeakMax)
				mvPeakMax = mvPeak;
		}
	);
	m_vertCoordVolt.Adjust(-mvPeakMax * 0.1f, mvPeakMax * 1.1f, 0._fPixel,fPixHeight);
}

fMicroSecs EventViewer::scanStartTime() const
{
	fMicroSecs us { fMicroSecs::NULL_VAL() };
	m_pNMWI->Apply2allEvents(EventType::startScan, [&us](NNetEvent const* e){ us = e->GetTimeStamp(); });
	assert(us.IsNotNull());
	return us;
}

fMicroSecs EventViewer::scanStopTime() const
{
	fMicroSecs us { fMicroSecs::NULL_VAL() };
	m_pNMWI->Apply2allEvents(EventType::stopScan, [&us](NNetEvent const* e){ us = e->GetTimeStamp(); });
	assert(us.IsNotNull());
	return us;
}

bool EventViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetTimeGraph::OnSize(width, height);
	adjust(PixelRectSize(width, height));
	return true;
}

// EventViewer.cpp
//
// NNetSignals

module;

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
	fMicroSecs usStartScan;
	fMicroSecs usStopScan;
	for (auto const& e : m_pNMWI->GetEventList())
	{
		if (e->Type() == EventType::startScan)
			usStartScan = e->GetTimeStamp();
		else if (e->Type() == EventType::stopScan)
			usStopScan = e->GetTimeStamp();
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
	for (auto const& e : m_pNMWI->GetEventList())
	{
		if (e->Type() == EventType::stimulus)
		{
			StimulusEvent   const* pStimEvent { static_cast<StimulusEvent const*>(e.get()) };
			SigGenId        const  sigGenId   { pStimEvent->GetId() };
			SignalGenerator const* pSigGen    { m_pNMWI->GetSigGenC(sigGenId) };
			fMicroSecs      const  usStimulus { pStimEvent->GetTimeStamp() };
			fMicroSecs      const  usStart    { max(0._MicroSecs, usStimulus - usStartScan) };
			PaintVoltCurve(pSigGen, usStart);
			bStimulus = true;
		}
	}
	if (!bStimulus)
		m_upGraphics->DisplayText(L"No stimulus");
};

void EventViewer::adjust(PixelRectSize const clientSize)
{
	if (m_pNMWI == nullptr)
		return;

	EventList  const& list    { m_pNMWI->GetEventList() };
	fMicroSecs const  usStart { list.front()->GetTimeStamp() };
	fMicroSecs const  usEnd   { list.back ()->GetTimeStamp() };
	m_horzCoord.Adjust(0._MicroSecs, usEnd - usStart, 0._fPixel, Convert2fPixel(clientSize.GetX()));

	mV mvPeakMax { 0._mV };
	for (auto const& e : list)
	{
		if (e->Type() == EventType::stimulus)
		{
			StimulusEvent   const* pStimEvent   { static_cast<StimulusEvent const*>(e.get()) };
			SigGenId        const  sigGenId     { pStimEvent->GetId() };
			SignalGenerator const* pSigGen      { m_pNMWI->GetSigGenC(sigGenId) };
			BasePeak<mV>    const& voltBasePeak { pSigGen->Amplitude() };
			mV              const  mvPeak       { voltBasePeak.Peak() };
			if (mvPeak > mvPeakMax)
				mvPeakMax = mvPeak;
		}
	}
	m_vertCoordVolt.Adjust(-mvPeakMax * 0.1f, mvPeakMax * 1.1f, 0._fPixel, Convert2fPixel(clientSize.GetY()));
}

bool EventViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetTimeGraph::OnSize(width, height);
	adjust(PixelRectSize(width, height));
	return true;
}

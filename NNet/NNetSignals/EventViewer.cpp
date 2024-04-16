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
	SetHorzCoord(&m_horzCoord);
	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 500000._MicroSecs);
	SetBackgroundColorRef(D2D1::ColorF::Green);
	BringWindowToTop();
}

void EventViewer::PaintGraphics()
{
	PixelRect rectxxx = GetWindowRect();
	fPixelRect rect;
	rect.SetBottom(GetClientHeight());
	for (auto const& e : m_pNMWI->GetEventList())
	{
		if (e->Type() == EventType::startScan)
			rect.SetLeft(Scale2pixelTime(e->GetTimeStamp()));
		else if (e->Type() == EventType::stopScan)
			rect.SetRight(Scale2pixelTime(e->GetTimeStamp()));
	}
	m_upGraphics->FillRectangle(rect, D2D1::ColorF::DarkGray);
	for (auto const& e : m_pNMWI->GetEventList())
	{
		if (e->Type() == EventType::stimulus)
		{
			StimulusEvent   const* pStimEvent { static_cast<StimulusEvent const*>(e.get()) };
			SigGenId        const  sigGenId   { pStimEvent->GetId() };
			SignalGenerator const* pSigGen    { m_pNMWI->GetSigGenC(sigGenId) };
			fMicroSecs      const  umStimulus { pStimEvent->GetTimeStamp() };
			PaintFreqCurve(pSigGen, umStimulus);
			PaintVoltCurve(pSigGen, umStimulus);
		}
	}
};

bool EventViewer::OnSize(PIXEL const pixClientWidth, PIXEL const pixClientHeight)
{
	if (m_pNMWI == nullptr)
		return false;
	EventList  const& list    { m_pNMWI->GetEventList() };
	fMicroSecs const  usStart { list.front()->GetTimeStamp() };
	fMicroSecs const  usEnd   { list.back ()->GetTimeStamp() };
	m_horzCoord.Adjust(usStart, usEnd, 0._fPixel, Convert2fPixel(pixClientWidth));
	return true;
}

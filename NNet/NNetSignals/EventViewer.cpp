// EventViewer.cpp
//
// NNetSignals

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
}

void EventViewer::PaintGraphics()
{
	for (auto const& e : m_pNMWI->GetEventList())
	{
		switch (e->Type())
		{
		case EventType::run:
			break;
		case EventType::stimulus:
			{
				StimulusEvent   const* pStimEvent { static_cast<StimulusEvent const*>(e.get()) };
				SigGenId        const  sigGenId   { pStimEvent->GetId() };
				SignalGenerator const* pSigGen    { m_pNMWI->GetSigGenC(sigGenId) };
				PaintFreqCurve(pSigGen, pStimEvent->GetTimeStamp());
				PaintVoltCurve(pSigGen, pStimEvent->GetTimeStamp());
			}
			break;
		case EventType::startScan:
			break;
		case EventType::stopScan:
			break;
		}
	}
};

bool EventViewer::OnSize(PIXEL const pixClientWidth, PIXEL const pixClientHeight)
{
	EventList  const& list    { m_pNMWI->GetEventList() };
	fMicroSecs const  usStart { (*list.begin())->GetTimeStamp() };
	fMicroSecs const  usEnd   { (*list.end  ())->GetTimeStamp() };
	m_horzCoord.Adjust(usStart, usEnd, 0._fPixel, Convert2fPixel(pixClientWidth));
	return true;
}

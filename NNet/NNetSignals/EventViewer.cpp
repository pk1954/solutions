// EventViewer.cpp
//
// NNetSignals

module NNetSignals:EventViewer;

import Types;
import NNetModel;

EventViewer::EventViewer
(
	HWND const hwndParent
)
  : NNetTimeGraph(hwndParent, L"ClassEventViewer")
{

}

void EventViewer::PaintGraphics()
{
	EventList  const& list    { m_pNMWI->GetEventList() };
	fMicroSecs const  usStart { (*list.begin())->GetTimeStamp() };
	fMicroSecs const  usEnd   { (*list.end  ())->GetTimeStamp() };
	fMicroSecs const  usDiff  { usEnd - usStart };
	for (auto const& e : list)
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
				PaintFreqCurve(pSigGen);
				PaintVoltCurve(pSigGen);
			}
			break;
		case EventType::startScan:
			break;
		case EventType::stopScan:
			break;
		}
	}
};

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
				StimulusEvent const *pStimEvent { static_cast<StimulusEvent const*>(e.get()) };
				SigGenId      const  sigGenId   { pStimEvent->GetId() };
				//PaintCurve
				//(
				//	[this](fMicroSecs const t){ return pixPntStimulusFreq(t); }, 
				//	0.0_MicroSecs,
				//	GetTime(xRight()),
				//	m_upGraphics->CreateBrush(D2D1::ColorF::Black),
				//	STD_WIDTH
				//);
			}
			break;
		case EventType::startScan:
			break;
		case EventType::stopScan:
			break;
		}
	}
};

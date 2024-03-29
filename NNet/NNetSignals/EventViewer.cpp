// EventViewer.cpp
//
// NNetSignals

module NNetSignals:EventViewer;

import Types;
import NNetModel;

void EventViewer::PaintGraphics()
{
	EventList const& list { m_pNMWI->GetEventList() };
	fMicroSecs usStart { (*list.begin())->GetTimeStamp() };
	fMicroSecs usEnd   { (*list.end  ())->GetTimeStamp() };
};

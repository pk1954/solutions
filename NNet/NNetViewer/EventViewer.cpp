// EventViewer.cpp
//
// NNetViewer

module;

#include <Windows.h>

module EventViewer;

import std;
import Types;
import PixFpDimension;
import IoConstants;
import NNetModel;

using std::wstring;
using std::wostringstream;
using std::streamsize;
using std::setprecision;
using std::fixed;
using std::endl;

EventViewer::EventViewer
(
	HWND const hwndParent,
	NNetModelReaderInterface const * const pNMRI,
	mV const& mVmaxAmplitude     
)
  : NNetTimeGraph(hwndParent, L"ClassEventViewer"),
	m_mVmaxAmplitude(mVmaxAmplitude)    
{
	SetModelInterface(pNMRI);
	SetHorzCoord    (&m_horzCoord);
	SetVertCoordVolt(&m_vertCoordVolt);
	m_horzCoord.SetPixelSizeLimits(10._MicroSecs, 500000._MicroSecs);
	m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
	//assert(pNMRI->AnyScanEvents());
	wostringstream wstrBuffer;
	wstrBuffer << L"Scan Protocol:";
	pNMRI->Apply2allEvents
	(
		[&wstrBuffer, pNMRI](NNetEvent const* pEvent)
		{
			fMicroSecs umScanTime { pNMRI->RelativeScanTime(*pEvent) };
			float      fSeconds   { umScanTime.GetValue() / 1000000.0f };
			wstrBuffer << endl << fixed << setprecision(2) << fSeconds << L's';
			wstrBuffer << SPACE << pEvent->GetEventTypeName();
			if (pEvent->Type() == EventType::stimulus)
			{
				StimulusEvent   const *pStimulusEvent { static_cast<StimulusEvent const *>(pEvent) };
			    SigGenId        const  idSigGen       { pStimulusEvent->GetId() };
				SignalGenerator const *pSigGen        { pNMRI->GetSigGenC(idSigGen) };
				wstrBuffer << SPACE << pSigGen->GetName();
			}
 		}
	);   
	m_upToolTip = CreateWindowToolTip(wstrBuffer.str());
	m_upToolTip->SetDuration(30000_MilliSecs);
}

bool EventViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetTimeGraph::OnSize(width, height);
	m_upToolTip->Resize();
	return true;
}

PIXEL EventViewer::GetFixedHeight() const 
{ 
	return GetParentRootWindow()->GetClientWindowHeight() / 10; 
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
	
	bool bStimulus = false;
	if (m_mVmaxAmplitude.IsNotZero())
	{
		adjust(GetWindowSize());
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
	}

	if (!bStimulus)
		m_upGraphics->DisplayText(L"No stimulus");
};

mV EventViewer::CalcMaxAmplitude() const
{
	mV mVpeakMax { 0.0_mV };
	m_pNMRI->Apply2allEvents
	(
		EventType::stimulus,
		[this, &mVpeakMax](StimulusEvent const* pStimEvent)
		{
			SignalGenerator const* pSigGen { m_pNMRI->GetSigGenC(pStimEvent->GetId()) };
			mV              const  mVpeak  { pSigGen->Amplitude().Peak() };
			mVpeakMax = max(mVpeak, mVpeakMax);
		}
	);
	return mVpeakMax;
}

void EventViewer::adjust(PixelRectSize const clientSize)
{
	if (m_pNMRI && m_mVmaxAmplitude.IsNotNull())
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
	m_vertCoordVolt.Adjust
	(
	   -m_mVmaxAmplitude * 0.1f, // - 10% of max peak 
	    m_mVmaxAmplitude * 1.1f, // 110% of max peak
		0._fPixel, 
		fPixHeight
	);
}

fMicroSecs EventViewer::scanTime(EventType const t) const
{
	fMicroSecs us { fMicroSecs::NULL_VAL() };
	m_pNMRI->Apply2allEvents(t, [&us](NNetEvent const* e){ us = e->GetTime(); });
	return us;
}

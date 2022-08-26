// MonitorControl.cpp
//
// NNetWindows

module;

#include <Windows.h>
#include "dwrite.h"
#include "Resource.h"
#include "SignalNr.h"
#include "win32_util_resource.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetColors.h"

module MonitorControl;

import BasicTypes;
import NNetModelCommands;
import SimulationTime;
import SignalId;

using std::to_wstring;

MonitorControl::MonitorControl
(
	HWND                 const   hwndParent,
	Sound                      & sound,
	NNetModelCommands          & modelCommands,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<mV>         & vertCoord
) 
  : TimeGraph(hwndParent, &horzCoord),
	m_horzCoord    (horzCoord),
	m_vertCoord    (vertCoord),
	m_sound        (sound),
	m_modelCommands(modelCommands)
{
	GraphicsWindow::Initialize(hwndParent, L"ClassMonitorControl", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
	m_measurement.Initialize(m_upGraphics.get());
	m_pTextFormat = m_upGraphics->NewTextFormat(16.f);
	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	m_hCrsrNS = LoadCursor(nullptr, IDC_SIZENS);
	m_hCrsrWE = LoadCursor(nullptr, IDC_SIZEWE);

	m_vertCoord.SetPixelSize(0.2_mV);
	m_vertCoord.SetPixelSizeLimits(0.001_mV, 100._mV);   
	m_vertCoord.SetZoomFactor(1.3f);

	m_horzCoord.RegisterObserver(*this);
	m_vertCoord.RegisterObserver(*this);
}

void MonitorControl::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	TimeGraph::SetModelInterface(pNMWI);
	m_pMonitorData = &m_pNMWI->GetMonitorData();
}

void MonitorControl::Reset()
{
	if (m_pMonitorData)
		m_pMonitorData->Reset();
	m_sound.Play(TEXT("DISAPPEAR_SOUND"));
}

void MonitorControl::Stop()
{
	GraphicsWindow::Stop();
}

LPARAM MonitorControl::AddContextMenuEntries(HMENU const hPopupMenu)
{
	if (m_measurement.IsActive())
		AppendMenu(hPopupMenu, MF_STRING, IDD_MEASUREMENT_OFF, L"Measurement off");
	else
		AppendMenu(hPopupMenu, MF_STRING, IDD_MEASUREMENT_ON,  L"Measurement on");

	if (m_trackNrHighlighted.IsNotNull() && m_pMonitorData->IsEmptyTrack(m_trackNrHighlighted))
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_TRACK, L"Delete track");

	if (m_pMonitorData->AnyEmptyTracks())
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_EMPTY_TRACKS, L"Delete empty tracks");

	if (m_trackNrHighlighted.IsNotNull())
		AppendMenu(hPopupMenu, MF_STRING, IDD_ADD_TRACK, L"Add track");

	if (m_pMonitorData->IsAnySignalSelected())
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL, L"Delete signal");

	AppendMenu(hPopupMenu, MF_STRING, IDD_SCALE_EEG_SIGNALS, L"Auto scale");
	
	return 0L; // will be forwarded to HandleContextMenuCommand
}

void MonitorControl::selectSignal(PixelPoint const &pixCrsrPos)
{
	if (m_measurement.Select(Convert2fPixel(pixCrsrPos.GetX())))
	{
		SetCursor(m_hCrsrWE);
	}
	else
	{
		SignalNr const signalFound { findSignal(m_trackNrHighlighted, pixCrsrPos) };
		if (signalFound.IsNotNull())
		{
			highlightSignal(SignalId(m_trackNrHighlighted, signalFound));
	//		SetCursor(m_hCrsrNS);
		}
	}
}

void MonitorControl::selectTrack(PixelPoint const &pixCrsrPos)
{
	TrackNr const trackNrFound { findTrack(pixCrsrPos.GetY()) };
	if (trackNrFound != m_trackNrHighlighted)
		m_trackNrHighlighted = trackNrFound;
}

SignalNr MonitorControl::findSignal
(
	TrackNr    const   trackNr, 
	PixelPoint const & ptCrsr
) const
{
	SignalNr signalNrRes { SignalNr::NULL_VAL() };

	if (m_pMonitorData->NoTracks() || trackNr.IsNull())
		return signalNrRes;

	fPixelPoint const fPixPtCrsr    { Convert2fPixelPoint(ptCrsr) };
	fMicroSecs  const usSimuTime    { - m_horzCoord.Transform2logUnitPos(fPixPtCrsr.GetX()) };
	fPixel            fPixBestDelta { fPixel::MAX_VAL() };
	m_pMonitorData->Apply2AllSignalsInTrackC
	(
		trackNr,
		[this, &trackNr, &usSimuTime, &fPixPtCrsr, &fPixBestDelta, &signalNrRes](SignalNr const signalNr)
		{
			SignalId idSignal { SignalId(trackNr, signalNr) };
			if (Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) })
			{
				if (usSimuTime >= pSig->GetStartTime())
				{
					fPixel const fPixSignal { getSignalValue(*pSig, usSimuTime) };
					if (fPixSignal.IsNotNull())
					{
						fPixel const fPixYpos     { getSignalOffset(idSignal) - fPixSignal };
						fPixel const fPixDelta    { fPixPtCrsr.GetY() - fPixYpos };
						fPixel const fPixDeltaAbs { fPixDelta.GetAbs() };
						if (fPixDeltaAbs < fPixBestDelta)
						{
							fPixBestDelta = fPixDeltaAbs;
							signalNrRes = signalNr;
						}
					}
				}
			}
		}
	);
	return signalNrRes;
}

TrackNr MonitorControl::findTrack(PIXEL const pixPosY) const
{
	fPixel  const fPixTrackHeight { calcTrackHeight() };
	fPixel  const fPixCrsrYpos    { Convert2fPixel(pixPosY) };
	TrackNr const trackNr         { Cast2Int(fPixCrsrYpos / fPixTrackHeight) };
	return m_pMonitorData->IsValid(trackNr) ? trackNr : TrackNr::NULL_VAL();
}

TrackNr MonitorControl::findPos4NewTrack(PIXEL const pixCrsrPosY) const
{
	fPixel const fPixTrackHeight  { calcTrackHeight() };
	fPixel const fPixCrsrYpos     { Convert2fPixel(pixCrsrPosY) };
	int    const iTrackNr         { Cast2Int(fPixCrsrYpos / fPixTrackHeight) };
	fPixel const fPixTrackCenterY { fPixTrackHeight * (Cast2Float(iTrackNr) + 0.5f) };
	TrackNr      trackNr          { m_trackNrHighlighted };
	assert(trackNr.IsNotNull());
	if (fPixCrsrYpos > fPixTrackCenterY)
		++trackNr;
	return trackNr;
}

void MonitorControl::highlightSignal(SignalId const & idNew)
{
	if (! m_pMonitorData->IsSelected(idNew))
	{
		if (m_pMonitorData->IsValid(idNew))
		{
			m_pMonitorData->SetHighlightedSignal(idNew);
		}
		else
		{
			m_pMonitorData->ResetHighlightedSignal();
			m_pixLast.Set2Null();
		}
		m_pixMoveOffsetY = 0_PIXEL;
	}
}

fPixel MonitorControl::getSignalOffset(SignalId const & idSignal) const
{ 
	fPixel fPixOffset { calcTrackHeight() * Cast2Float(idSignal.GetTrackNr().GetValue()+1) }; 
	if (m_pMonitorData->IsSelected(idSignal))  
		fPixOffset += Convert2fPixel(m_pixMoveOffsetY);  // may have move offset
	return fPixOffset;
}

fPixel MonitorControl::getSignalValue
(
	Signal     const & signal, 
	fMicroSecs const   usSimu
) const
{
	mV const mVsignal { signal.GetDataPoint(m_pNMWI->GetParams(), usSimu) };
	return (mVsignal.IsNull())
		? fPixel::NULL_VAL()
		: m_vertCoord.Transform2fPixelSize(mVsignal);
}

fPixel MonitorControl::calcTrackHeight() const
{
	fPixel const fPixRectHeight  { Convert2fPixel(GetClientWindowHeight()) };
	fPixel       fPixTrackHeight { fPixRectHeight };
	if (m_pMonitorData)
	{ 
		int const iNrOfTracks { m_pMonitorData->GetNrOfTracks() };
		if (iNrOfTracks > 0)
			fPixTrackHeight /= Cast2Float(iNrOfTracks);
	}; 
	return fPixTrackHeight;
}

fPixelPoint MonitorControl::calcDiamondPos() const
{
	fPixelPoint      fPixDiamondPos { fPP_NULL };
	SignalId const & idSignal       { m_pMonitorData->GetHighlightedSignalId() };

	if (Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) })
	{
		if (getSignalValue(*pSig, SimulationTime::Get()).IsNotNull())
		{
			PixelPoint const pixPointCrsr { GetRelativeCrsrPosition() };
			fPixel     const fPixCrsrX    { Convert2fPixel(pixPointCrsr.GetX()) };
			fMicroSecs const usSimu       { pixel2simuTime(fPixCrsrX) };
			if (usSimu >= pSig->GetStartTime())
			{
				fMicroSecs const usSimuMax  { pSig->FindNextMaximum(m_pNMWI->GetParams(), usSimu) };
				fPixel     const fPixMaxX   { simu2pixelTime(usSimuMax) };
				fPixel     const fPixYoff   { getSignalOffset(idSignal) };
				fPixel     const fPixSigVal { getSignalValue(*pSig, usSimuMax) };
				if (fPixSigVal.IsNotNull())
					fPixDiamondPos = fPixelPoint(fPixMaxX, fPixYoff - fPixSigVal);
			}
		}
	}
	return fPixDiamondPos;
}

fPixelPoint MonitorControl::getSignalPoint
(
	Signal     const & signal, 
	fMicroSecs const   usSimu,
	fPixel     const   fPixYoff
) const
{
	return fPixelPoint(simu2pixelTime(usSimu), fPixYoff - getSignalValue(signal, usSimu));
}

void MonitorControl::paintWarningRect() const
{
	m_upGraphics->FillRectangle
	(
		fPixelRect
		(
			fPixelPoint   (m_fPixRightLimit, 0._fPixel),
			fPixelRectSize(m_fPixRightBorder, Convert2fPixel(GetClientWindowHeight()))
		), 
		NNetColors::COL_WARNING
	);
}

void MonitorControl::paintSignal(SignalId const & idSignal)
{
	Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) };
	if (pSig == nullptr)
		return;

	fPixel     const fPixHorzBegin { 0._fPixel };
	fPixel     const fPixHorzEnd   { m_fPixWinWidth - m_fPixRightBorder };

	fMicroSecs const usSimuBegin   { pixel2simuTime(fPixHorzBegin) };
	fMicroSecs const usSimuEnd     { pixel2simuTime(fPixHorzEnd) };

	fMicroSecs const usSimuZero    { pSig->GetStartTime() };
	fMicroSecs const usSimuNow     { SimulationTime::Get() };

	fMicroSecs const usSimuStart   { max(usSimuBegin, usSimuZero) };
	fMicroSecs const usSimuStop    { min(usSimuEnd  , usSimuNow) };

	fPixel     const fPixOffsetY   { getSignalOffset(idSignal) };
	D2D1::ColorF     color         { ColorF::Black };
	fPixel           fPixWidth     { 1.0_fPixel };

	if (getSignalValue(*pSig, usSimuStop).IsNull())
		return;

	if (m_pMonitorData->IsSelected(idSignal) && (m_pMonitorData->GetNrOfSignals() >1)) 
	{
		color     = NNetColors::EEG_SIGNAL_HIGH;  // emphasize selected signal 
		fPixWidth = 3.0_fPixel;                         
	}

	fPixel fPixMinSignal = PaintCurve
	(
		[this, fPixOffsetY, pSig](fMicroSecs const t) 
		{ 
			return getSignalPoint(*pSig, t, fPixOffsetY); 
		},
		usSimuStart, usSimuStop, fPixWidth, color          
	);

	fPixel fPixMaxSignal = fPixOffsetY - fPixMinSignal;
	if (fPixMaxSignal > m_fPixMaxSignal)
		m_fPixMaxSignal = fPixMaxSignal;

	fPixelPoint lastPoint { getSignalPoint(*pSig, usSimuNow, fPixOffsetY) };
	m_upGraphics->FillCircle(fPixelCircle(lastPoint, 4.0_fPixel), color);
}

bool MonitorControl::SignalTooHigh() const
{
	fPixel const fPixTrackHeight { calcTrackHeight() };
	return m_fPixMaxSignal > fPixTrackHeight;
}

float MonitorControl::ScaleFactor() const
{
	return (m_fPixMaxSignal > 0.0_fPixel) 
		   ? m_fPixMaxSignal / calcTrackHeight()
	       : 1.0f;
}
void MonitorControl::ScaleSignals()
{
	if (m_fPixMaxSignal > 0.0_fPixel) 
		m_vertCoord.ZoomFactor(ScaleFactor(), 0.0_fPixel);
}

void MonitorControl::paintNumber
(
	fPixel       const xPos,
	fPixel       const yPos,
	int          const iNr,
	D2D1::ColorF const col
) const
{
	m_upGraphics->DisplayText
	(
		fPixelRect
		(
			fPixelPoint   (xPos, yPos), 
			fPixelRectSize(30._fPixel, 10._fPixel)
		),
		to_wstring(iNr), 
		col, 
		m_pTextFormat
	);
}

void MonitorControl::paintTrack(TrackNr const trackNr) const
{
	ColorF const col 
	{
		(trackNr.GetValue() % 2)
		? ((trackNr == m_trackNrHighlighted) ? NNetColors::COL_TRACK_ODD_H  : NNetColors::COL_TRACK_ODD)
		: ((trackNr == m_trackNrHighlighted) ? NNetColors::COL_TRACK_EVEN_H : NNetColors::COL_TRACK_EVEN)
	};
	fPixel const fPixTrackHeight { calcTrackHeight() };
	fPixel const fPixTrackTop    { fPixTrackHeight * Cast2Float(trackNr.GetValue()) };

	m_upGraphics->FillRectangle
	(
		fPixelRect
		(
			fPixelPoint   (0._fPixel, fPixTrackTop),
			fPixelRectSize(m_fPixWinWidth, fPixTrackHeight)
		), 
		col
	);

	paintNumber(m_fPixWinWidth - 15._fPixel, fPixTrackTop, trackNr.GetValue(), D2D1::ColorF::Black);
}

void MonitorControl::StimulusTriggered() 
{ 
	m_pMonitorData->AddStimulus(SimulationTime::Get());
}

void MonitorControl::paintStimulusMarkers() const
{
	fPixel const fPixBottom { Convert2fPixel(GetClientWindowHeight()) };

	int iStimulusNr { 1 };
	for (auto it : m_pMonitorData->GetStimulusList())
	{
		fPixel      const fPixX     { simu2pixelTime(it) };
		fPixelPoint const fPixBegin { fPixX, 0._fPixel };
		fPixelPoint const fPixEnd   { fPixX, fPixBottom };
		m_upGraphics->DrawLine(fPixBegin, fPixEnd, 1._fPixel, NNetColors::COL_STIMULUS_LINE);
		paintNumber(fPixX + 2._fPixel, 0._fPixel, iStimulusNr, NNetColors::COL_STIMULUS_LINE);
		++iStimulusNr;
	}
}

void MonitorControl::DoPaint()
{
	m_fPixMaxSignal = 0.0_fPixel;

	if (m_pMonitorData->NoTracks())
		return;

	if (m_pMonitorData->GetNrOfTracks() > 1)
		m_pMonitorData->Apply2AllTracksC([this](TrackNr const n) { paintTrack(n); });

	m_pMonitorData->Apply2AllSignalIdsC([this](SignalId const id) { paintSignal(id); });

	if (SignalTooHigh())
		paintWarningRect();

	paintStimulusMarkers();

	m_measurement.DisplayDynamicScale(fMicroSecs(m_horzCoord.GetPixelSize()));

	if (m_measurement.TrackingActive())
	{
		fPixelPoint const fPixDiamondPos { calcDiamondPos() };
		if (fPixDiamondPos.IsNotNull())
			m_upGraphics->FillDiamond(fPixDiamondPos, 4.0_fPixel, NNetColors::COL_DIAMOND);
	}
}

bool MonitorControl::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	bool bRes = false;

	switch (int const wmId = LOWORD(wParam))
	{
	case IDD_MEASUREMENT_OFF:
		m_measurement.SetActive(false);
		break;

	case IDD_MEASUREMENT_ON:
		m_measurement.SetActive(true);
		break;

	case IDD_DELETE_SIGNAL:
		PostCommand2Application(wmId, 0);
		break;

	case IDD_ADD_TRACK:
		SendCommand2Application(wmId, static_cast<LPARAM>(findPos4NewTrack(pixPoint.GetY()).GetValue()));
		SendCommand(IDM_WINDOW_ON, 0);  // if window was not visible, show it now
		break;

	case IDD_DELETE_TRACK:
		if (m_pMonitorData->GetNrOfTracks()==1)
			PostCommand(IDM_WINDOW_OFF, 0);
		PostCommand2Application(wmId, static_cast<LPARAM>(m_trackNrHighlighted.GetValue()));
		break;

	case IDD_DELETE_EMPTY_TRACKS:
		m_pMonitorData->Apply2AllTracksRevC
		(
			[this](TrackNr const trackNr)
			{  
				if (m_pMonitorData->IsEmptyTrack(trackNr))
					PostCommand2Application(IDD_DELETE_TRACK, static_cast<LPARAM>(trackNr.GetValue()));
			}
		);
		break;

	case IDD_SCALE_EEG_SIGNALS:
		ScaleSignals();
		break;

	default:
	    bRes = BaseWindow::OnCommand(wParam, lParam, pixPoint);
		break;
	}

	return bRes; 
}

void MonitorControl::OnMouseLeave()
{
	if (! CrsrInClientRect())
		m_trackNrHighlighted.Set2Null();
	GraphicsWindow::OnMouseLeave();
}

bool MonitorControl::OnShow(WPARAM const wParam, LPARAM const lParam)
{
	if (static_cast<bool>(wParam))
		m_measurement.ResetLimits();
	return false;
}

void MonitorControl::moveOperation(PixelPoint const &pixCrsrPos)
{
	if (m_pixLast.IsNotNull())
	{
		if (m_measurement.TrackingActive())
		{
			fPixel fPixNewPos { Convert2fPixel(pixCrsrPos.GetX()) };
			if (fPixNewPos > m_fPixRightLimit)
				fPixNewPos = m_fPixRightLimit;
			m_measurement.MoveSelection(fPixNewPos);
			SetCursor(m_hCrsrWE);
		}
		else 
		{
			if (m_pMonitorData->IsAnySignalSelected())
			{
				m_pixMoveOffsetY += pixCrsrPos.GetY() - m_pixLast.GetY();
				SetCursor(m_hCrsrNS);
			}
		}
	}
	m_pixLast = pixCrsrPos;
}

void MonitorControl::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam(lParam) };

	if (! m_measurement.TrackingActive())
		selectTrack(pixCrsrPos);

	if (wParam & MK_LBUTTON)
		moveOperation(pixCrsrPos);
	else  // left button not pressed: select
		selectSignal(pixCrsrPos);

	Trigger();   // cause repaint

	GraphicsWindow::OnMouseMove(wParam, lParam);
}

void MonitorControl::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam) 
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam(lParam) };
	fPixel     const fPixCrsrX  { Convert2fPixel(pixCrsrPos.GetX()) };
	if (fPixCrsrX > m_fPixRightLimit)
	{
		if (SignalTooHigh())
			ScaleSignals();
	}
	else
	{
		fPixelPoint const fPixDiamondPos { calcDiamondPos() };
		if (
				fPixDiamondPos.IsNotNull() && m_pMonitorData->IsAnySignalSelected() &&
				(m_measurement.IsClose2LeftLimit(fPixCrsrX) || m_measurement.IsClose2RightLimit(fPixCrsrX))
			)
		{
			m_measurement.MoveSelection(fPixDiamondPos.GetX());
			m_sound.Play(TEXT("SNAP_IN_SOUND")); 
			Trigger();  // cause repaint
		}
	}
};

bool MonitorControl::OnLButtonUp(WPARAM const wParam, LPARAM const lParam) 
{
	if (
		(m_trackNrHighlighted.IsNotNull()) && 
		(m_pMonitorData->GetSelectedTrackNr() != m_trackNrHighlighted) && 
		(! m_measurement.TrackingActive()) &&
		(m_pMonitorData->IsAnySignalSelected())
	   )
	{
		m_modelCommands.MoveSignal(m_pMonitorData->GetHighlightedSignalId(), m_trackNrHighlighted);
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
	Trigger();  // cause repaint
	return GraphicsWindow::OnLButtonUp(wParam, lParam);
};

void MonitorControl::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		bResult = bShiftKey 
			? m_horzCoord.ZoomDir(bDirection, 0.0_fPixel)
			: m_vertCoord.ZoomDir(bDirection, 0.0_fPixel);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}

bool MonitorControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixWinWidth   = Convert2fPixel(width);
	m_fPixRightLimit = m_fPixWinWidth - m_fPixRightBorder;
	m_measurement.SetClientRectSize(width, height);
	return true;
}

void MonitorControl::Notify(bool const bImmediately)
{
	if (bImmediately)
		UpdateImmediately();
	else
		TimeGraph::Notify(bImmediately);
}

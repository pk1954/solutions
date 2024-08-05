// MonitorControl.cpp
//
// NNetSignals

module;

#include <string>
#include <cassert>
#include <vector>
#include <Windows.h>
#include "dwrite.h"
#include "Resource.h"

module NNetSignals:MonitorControl;

import Win32_Util_Resource;
import Types;
import Signals;
import WinManager;
import NNetCommands;

using std::vector;
using std::to_wstring;

static Color const COL_DIAMOND       { 0.0f, 1.0f, 0.0f, 1.0f };
static Color const COL_STIMULUS_LINE { 0.5f, 0.1f, 0.1f, 1.0f };
static Color const COL_TRACK_EVEN    { 0.9f, 0.9f, 0.9f, 1.0f };
static Color const COL_TRACK_EVEN_H  { 0.9f, 0.9f, 1.0f, 1.0f };
static Color const COL_TRACK_ODD     { 0.8f, 0.8f, 0.8f, 1.0f };
static Color const COL_TRACK_ODD_H   { 0.8f, 0.8f, 0.9f, 1.0f };
static Color const COL_WARNING       { 0.8f, 0.0f, 0.0f, 0.3f };
static Color const EEG_SIGNAL_HIGH   { 1.0f, 0.5f, 0.0f, 1.0f };

MonitorControl::MonitorControl
(
	HWND                 const   hwndParent,
	Sound                      & sound,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<mV>         & vertCoord,
	Observable                 & observable
)
  : NNetTimeGraph(hwndParent, L"ClassMonitorControl"),
	m_vertCoord(vertCoord),
	m_sound    (sound)
{
	SetHorzCoord(&horzCoord);
	m_measurement.Initialize(m_upGraphics.get());
	m_hTextFormat = m_upGraphics->NewTextFormat(16.f);
	m_hTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	m_hCrsrNS = LoadCursor(nullptr, IDC_SIZENS);
	m_hCrsrWE = LoadCursor(nullptr, IDC_SIZEWE);

	m_vertCoord.SetPixelSizeLimits(0.001_mV, 1000._mV);   
	m_vertCoord.SetPixelSize(0.2_mV);
	m_vertCoord.SetZoomFactor(1.3f);

	horzCoord  .RegisterObserver(*this);
	m_vertCoord.RegisterObserver(*this);

	m_hBrushNormal   = m_upGraphics->CreateBrushHandle(ColorF::Black);
	m_hBrushSelected = m_upGraphics->CreateBrushHandle(EEG_SIGNAL_HIGH);

	SetDefaultBackgroundColor();

	m_pObservable = &observable;
}

void MonitorControl::SetDefaultBackgroundColor()
{
	m_upGraphics->SetBackgroundColor(COL_TRACK_EVEN);
}

void MonitorControl::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
{
	NNetTimeGraph::SetModelInterface(pNMRI);
	m_pMonitorData = &m_pNMRI->GetMonitorDataC();
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

	if (IsAnySignalHighlighted())
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL, L"Delete signal");

	AppendMenu(hPopupMenu, MF_STRING, IDD_SCALE_EEG_SIGNALS, L"Auto scale");
	
	NNetTimeGraph::AddContextMenuEntries(hPopupMenu);

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

float MonitorControl::pixel2Track(PIXEL const pixPosY) const
{
	fPixel  const fPixTrackHeight { calcTrackHeight() };
	fPixel  const fPixCrsrYpos    { Convert2fPixel(pixPosY) };
	float   const fTrack          { fPixCrsrYpos / fPixTrackHeight };
	return fTrack;
}

void MonitorControl::selectTrack(PixelPoint const &pixCrsrPos)
{
	TrackNr trackNr { Cast2Int(pixel2Track(pixCrsrPos.GetY())) };
	if (!m_pMonitorData->IsValid(trackNr))
		trackNr.Set2Null();
	m_trackNrHighlighted = trackNr;
}

int MonitorControl::findTrackPos(PIXEL const pixPosY) const
{
	return Cast2Int(round(pixel2Track(pixPosY)));
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
	fMicroSecs  const usSimuTime    { pixel2simuTime(fPixPtCrsr.GetX()) };
	fPixel            fPixBestDelta { fPixel::MAX_VAL() };
	m_pMonitorData->Apply2AllSignalsInTrackC
	(
		trackNr,
		[this, &trackNr, &usSimuTime, &fPixPtCrsr, &fPixBestDelta, &signalNrRes](SignalNr const signalNr)
		{
			SignalId idSignal { SignalId(trackNr, signalNr) };
			if (Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) })
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
	);
	return signalNrRes;
}

void MonitorControl::highlightSignal(SignalId const & idNew)
{
	if (!IsSignalHighlighted(idNew))
	{
		if (m_pMonitorData->IsValid(idNew))
		{
			SetHighlightedSignal(idNew);
		}
		else
		{
			SetHighlightedSignal(SignalId::NULL_VAL());
			m_pixLast.Set2Null();
		}
		m_pixMoveOffsetY = 0_PIXEL;
	}
}

fPixel MonitorControl::getSignalOffset(SignalId const & idSignal) const
{ 
	fPixel const fPixTrackHeight { calcTrackHeight() };
	fPixel       fPixOffset      { fPixTrackHeight * Cast2Float(idSignal.GetTrackNr().GetValue()+1) };
	if (IsSignalHighlighted(idSignal))  
		fPixOffset += Convert2fPixel(m_pixMoveOffsetY);  // may have move offset
	return fPixOffset;
}

fPixel MonitorControl::getSignalValue
(
	Signal     const & signal, 
	fMicroSecs const   usSimu
) const
{
	mV const mVsignal { signal.GetDataPoint(m_pNMRI->GetSignalParamsC(), usSimu) };
	return (mVsignal.IsNull())
		? fPixel::NULL_VAL()
		: m_vertCoord.Transform2fPixelSize(mVsignal);
}

fPixel MonitorControl::calcTrackHeight() const
{
	fPixel const fPixRectHeight  { GetClientHeight() };
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
	SignalId const & idSignal       { GetHighlightedSignalId() };

	if (Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) })
	{
		if (getSignalValue(*pSig, SimulationTime::Get()).IsNotNull())
		{
			PixelPoint const pixPointCrsr { GetRelativeCrsrPosition() };
			fPixel     const fPixCrsrX    { Convert2fPixel(pixPointCrsr.GetX()) };
			fMicroSecs const usSimu       { pixel2simuTime(fPixCrsrX) };
			if (usSimu >= pSig->GetStartTime())
			{
				fMicroSecs const usSimuMax  { pSig->FindNextMaximum(m_pNMRI->GetSignalParamsC(), usSimu) };
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
	return fPixelPoint
	(
		simu2pixelTime(usSimu), 
		fPixYoff - getSignalValue(signal, usSimu) - 1.0_fPixel
	);
}

void MonitorControl::paintWarningRect() const
{
	m_upGraphics->FillRectangle
	(
		fPixelRect
		(
			fPixelPoint   (m_fPixRightLimit, 0._fPixel),
			fPixelRectSize(xRightBorder(), GetClientHeight())
		), 
		COL_WARNING
	);
}

void MonitorControl::paintSignal(SignalId const & idSignal)
{
	Signal const * pSig { m_pMonitorData->GetConstSignalPtr(idSignal) };
	if (pSig == nullptr)
		return;

	fPixel          const fPixHorzBegin { 0._fPixel };
	fPixel          const fPixHorzEnd   { m_fPixWinWidth - xRightBorder() };
			       
	fMicroSecs      const usSimuBegin   { pixel2simuTime(fPixHorzBegin) };
	fMicroSecs      const usSimuEnd     { pixel2simuTime(fPixHorzEnd) };
			       
	fMicroSecs      const usSimuZero    { pSig->GetStartTime() };
	fMicroSecs      const usSimuNow     { SimulationTime::Get() };
			       
	fMicroSecs      const usSimuStart   { max(usSimuBegin, usSimuZero) };
	fMicroSecs      const usSimuStop    { min(usSimuEnd  , usSimuNow) };
			       
	fPixel          const fPixOffsetY   { getSignalOffset(idSignal) };
	BrushHandle           hBrush        { m_hBrushNormal };
	fPixel                fPixWidth     { STD_WIDTH };

	if (getSignalValue(*pSig, usSimuStop).IsNull())
		return;

	if (IsSignalHighlighted(idSignal) && (m_pMonitorData->GetNrOfSignals() >1)) 
	{
		hBrush = m_hBrushSelected;  // emphasize selected signal 
		fPixWidth = HIGH_WIDTH;
	}

	fPixel fPixMinSignal = Paint
	(
		[this, fPixOffsetY, pSig](fMicroSecs const t)
		{ 
			return getSignalPoint(*pSig, t, fPixOffsetY); 
		},
		usSimuStart, usSimuStop, 
		GetParams()->TimeResolution(),
		hBrush,
		fPixWidth
	);

	fPixel fPixMaxSignal = fPixOffsetY - fPixMinSignal;
	if (fPixMaxSignal > m_fPixMaxSignal)
		m_fPixMaxSignal = fPixMaxSignal;

	fPixelPoint fPixPntSignalNow { getSignalPoint(*pSig, usSimuNow, fPixOffsetY) };
	m_upGraphics->FillCircle(fPixelCircle(fPixPntSignalNow, 4.0_fPixel), hBrush);
	
	// paint block times

	NNetSignal       const* pNNetSig { static_cast<NNetSignal const*>(pSig) };
	NNetSignalSource const& sigSrc   { * pNNetSig->GetSignalSource() };
	if (sigSrc.SignalSourceType() != NNetSignalSource::Type::microSensor)
		return;

	MicroSensor const& microSensor { static_cast<MicroSensor const &>(sigSrc) };
	NobId       const  idNob       { microSensor.GetNobId() };
	Nob         const& nob         { *m_pNMRI->GetConstNob(idNob) };
	if (!nob.IsSynapse())
		return;

	vector<fMicroSecs> const& list { Cast2Synapse(&nob)->GetBlockList() };

	fPixel const fPixTrackHeight { calcTrackHeight() };

	int index { 0 };
	while (index < list.size())
	{
		fMicroSecs usBlockStart { list[index++] };
		fMicroSecs usBlockEnd   { (index == list.size()) ? usSimuStop : list[index++] };
		if (usBlockEnd >= usSimuStart)
		{
			fPixelRect rect
			{
				simu2pixelTime(usBlockStart),      // left
				fPixOffsetY - fPixTrackHeight / 2, // top
				simu2pixelTime(usBlockEnd),        // right
				fPixOffsetY - fPixTrackHeight / 4  // bottom
			};
			m_upGraphics->FillRectangle(rect, D2D1::ColorF::DarkGray);
		}
	}
}

PixelPoint MonitorControl::GetTrackPosScreen(SignalId const signalId, tHorzDir const dir) const
{
	Signal const* pSig { m_pMonitorData->GetConstSignalPtr(signalId) };
	if (pSig == nullptr)
		return PixelPoint::NULL_VAL();
	fPixel      const fPixTrackHeight { calcTrackHeight() };
	fPixel      const fPixVertPos     { getSignalOffset(signalId) - fPixTrackHeight * 0.5f };
	fPixel      const fPixHorzPos     { (dir == tHorzDir::right) ? m_fPixWinWidth : 0.0_fPixel };
	fPixelPoint const fPixPntSignal	  { fPixHorzPos, fPixVertPos };
	PixelPoint  const pixPntSignal    { Convert2PixelPoint(fPixPntSignal) };
	PixelPoint  const pixPosScreen    { Client2Screen(pixPntSignal) };

	//TrackNr trackNr      { signalId.GetTrackNr() };
	//size_t  iNrOfSignals { m_pMonitorData->GetNrOfSignals(trackNr) };
	// TODO: vertical offset, sort handles to avoid cable intersection

	return pixPosScreen;
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
	fPixel const xPos,
	fPixel const yPos,
	int    const iNr,
	Color  const col
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
		m_hTextFormat
	);
}

void MonitorControl::paintTrack(TrackNr const trackNr) const
{
	Color const col 
	{
		(trackNr.GetValue() % 2)
		? ((trackNr == m_trackNrHighlighted) ? COL_TRACK_ODD_H  : COL_TRACK_ODD)
		: ((trackNr == m_trackNrHighlighted) ? COL_TRACK_EVEN_H : COL_TRACK_EVEN)
	};
	fPixel const fPixTrackHeight { calcTrackHeight() };
	fPixel const fPixTrackTop    { fPixTrackHeight * Cast2Float(trackNr.GetValue()) };

	m_upGraphics->FillRectangle
	(
		fPixelRect
		(
			fPixelPoint   (0._fPixel, fPixTrackTop),
			fPixelRectSize(m_fPixWinWidth+1, fPixTrackHeight)
		), 
		col
	);

	paintNumber(m_fPixWinWidth - 15._fPixel, fPixTrackTop, trackNr.GetValue(), D2D1::ColorF::Black);
}

void MonitorControl::paintStimulusMarkers() const
{
	fPixel const fPixBottom { GetClientHeight() };

	int iStimulusNr { 1 };
	m_pNMRI->Apply2allEvents
	(
		EventType::stimulus,
		[this, fPixBottom, &iStimulusNr](StimulusEvent const* pStimEvent)
		{
			fMicroSecs  const usStimulus { pStimEvent->GetTime() };
			fPixel      const fPixX      { simu2pixelTime(usStimulus) };
			fPixelPoint const fPixBegin  { fPixX, 0._fPixel };
			fPixelPoint const fPixEnd    { fPixX, fPixBottom };
			m_upGraphics->DrawLine(fPixBegin, fPixEnd, 1._fPixel,  COL_STIMULUS_LINE);
			paintNumber(fPixX + 2._fPixel, 0._fPixel, iStimulusNr, COL_STIMULUS_LINE);
	        ++iStimulusNr;
		}
	);   
}

void MonitorControl::PaintGraphics()
{
	m_fPixMaxSignal = 0.0_fPixel;
	m_pMonitorData->Apply2AllTracksC   ([this](TrackNr  const n ) { paintTrack (n); });
	m_pMonitorData->Apply2AllSignalIdsC([this](SignalId const id) { paintSignal(id); });

	if (SignalTooHigh())
		paintWarningRect();

	paintStimulusMarkers();

	m_measurement.DisplayDynamicScale(fMicroSecs(GetHorzCoord()->GetPixelSize()));

	if (m_measurement.TrackingActive())
	{
		fPixelPoint const fPixDiamondPos { calcDiamondPos() };
		if (fPixDiamondPos.IsNotNull())
			m_upGraphics->FillDiamond(fPixDiamondPos, 4.0_fPixel, COL_DIAMOND);
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
		deleteSignal();
		break;

	case IDD_ADD_TRACK:
		WinManager::SendCommand2App(wmId, static_cast<LPARAM>(findTrackPos(pixPoint.GetY())));
		SendCommand(IDM_WINDOW_ON, 0);  // if window was not visible, show it now
		break;

	case IDD_DELETE_TRACK:
		if (m_pMonitorData->GetNrOfTracks()==1)
			PostCommand(IDM_WINDOW_OFF, 0);
		WinManager::PostCommand2App(wmId, static_cast<LPARAM>(m_trackNrHighlighted.GetValue()));
		break;

	case IDD_DELETE_EMPTY_TRACKS:
		m_pMonitorData->Apply2AllTracksRevC
		(
			[this](TrackNr const trackNr)
			{  
				if (m_pMonitorData->IsEmptyTrack(trackNr))
					WinManager::PostCommand2App(IDD_DELETE_TRACK, static_cast<LPARAM>(trackNr.GetValue()));
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

void MonitorControl::MoveHighlightedSignal(PIXEL const pixDelta)
{
	m_pixMoveOffsetY += pixDelta;
	m_pObservable->NotifyAll();
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
		}
		else if (IsAnySignalHighlighted())
		{
			MoveHighlightedSignal(pixCrsrPos.GetY() - m_pixLast.GetY());
		}
	}
	m_pixLast = pixCrsrPos;
}

void MonitorControl::deleteSignal()
{
    Signal           const* pSignal     { GetHighlightedSignal() };
    NNetSignal       const* pNNetSignal { static_cast<NNetSignal const*>(pSignal) };
    NNetSignalSource const* pSigSrc     { pNNetSignal->GetSignalSource() };
    switch (pSigSrc->SignalSourceType())
    {
        using enum NNetSignalSource::Type;
        case macroSensor:
        {
            Sensor const* pSensor  { static_cast<Sensor const*>(pSigSrc) };
            SensorId      idSensor { m_pNMRI->GetSensorList().GetSensorId(*pSensor) };
            DelSensorCmd::Push(idSensor);
            break;
        }
        case microSensor:
        {
            MicroSensor const * pMicroSensor { static_cast<MicroSensor const*>(pSigSrc) };
            NobId       const   idNob        { pMicroSensor->GetNobId() };
            DelMicroSensorCmd::Push(idNob);
            break;
        }
    }
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
    fPixelPoint const fPixCrsrPos { GetCrsrPosFromLparamF(lParam) };
	fPixel      const fPixCrsrX   { fPixCrsrPos.GetX() };
	if (fPixCrsrX > m_fPixRightLimit)
	{
		if (SignalTooHigh())
			ScaleSignals();
	}
	else
	{
		fPixelPoint const fPixDiamondPos { calcDiamondPos() };
		if (
				fPixDiamondPos.IsNotNull() && IsAnySignalHighlighted() &&
				(m_measurement.IsClose2LeftLimit(fPixCrsrX) || m_measurement.IsClose2RightLimit(fPixCrsrX))
			)
		{
			m_measurement.MoveSelection(fPixDiamondPos.GetX());
			m_sound.Play(TEXT("SNAP_IN_SOUND")); 
			Trigger();  // cause repaint
		}
	}
};

void MonitorControl::DropSignal()
{
	if (int const iTrackDelta = findTrackPos(m_pixMoveOffsetY))
	{
		SignalId signalId   { GetHighlightedSignalId() };
		TrackNr  trackNrNew { signalId.GetTrackNr() + iTrackDelta };
		if (m_pMonitorData->IsValid(trackNrNew))
			MoveSignalCmd::Push(signalId, trackNrNew);
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
	Trigger();  // cause repaint
};

bool MonitorControl::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	if (!m_measurement.TrackingActive() && IsAnySignalHighlighted())
		DropSignal();
	return GraphicsWindow::OnLButtonUp(wParam, lParam);
};

bool MonitorControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixWinWidth   = Convert2fPixel(width);
	m_fPixRightLimit = m_fPixWinWidth - xRightBorder();
	m_measurement.SetClientRectSize(width, height);
	return true;
}

void MonitorControl::Notify(bool const bImmediately)
{
	if (bImmediately)
		UpdateImmediately();
	else
		NNetTimeGraph::Notify(bImmediately);
}

SignalId MonitorControl::SetHighlightedSignal(SignalId const id)
{
	SignalId const signalIdOld { m_idSigHighlighted };
	m_idSigHighlighted = id;
	if (m_pHighSigObservable)
		m_pHighSigObservable->NotifyAll();
	return signalIdOld;
}

SignalId MonitorControl::SetHighlightedSignal(Signal const & sigNew)
{
	return SetHighlightedSignal(m_pMonitorData->FindSignalId([&sigNew](Signal const &s){ return &s == &sigNew; }));
}

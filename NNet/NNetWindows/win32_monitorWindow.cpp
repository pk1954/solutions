// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include <assert.h>
#include "util.h"
#include "win32_scale.h"
#include "PixCoordFp.h"
#include "Signal.h"
#include "Track.h"
#include "Resource.h"
#include "BaseKnot.h"
#include "MonitorData.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "NNetModelReaderInterface.h"
#include "win32_monitorWindow.h"

using std::find;

void MonitorWindow::Start
(
	HWND                     const   hwndParent,
	Sound                  * const   pSound,
	NNetController         * const   pController,
	NNetModelCommands      * const   pModelCommands,
	NNetModelReaderInterface const & nmri,
	MonitorData                    & monitorData 
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		L"ClassMonitorWindow",
		WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
		nullptr,
		nullptr
	);
	m_pSound         =   pSound;
	m_pController    =   pController;
	m_pModelCommands =   pModelCommands;
	m_pNMRI          = & nmri;
	m_pMonitorData   = & monitorData;
	m_graphics.Initialize(hwnd);
	SetWindowText(hwnd, L"Monitor");
	m_trackStruct.hwndTrack = hwnd;
	m_measurement.Initialize(& m_graphics);

	m_horzCoord.SetPixelSize(100.0_MicroSecs); 
	m_horzCoord.SetPixelSizeLimits(1._MicroSecs, 4000._MicroSecs); 
	m_horzCoord.SetZoomFactor(1.3f);
	m_horzCoord.RegisterObserver(this);

	//m_horzScale.InitHorzScale(&m_horzCoord, &m_graphics, L"s", 1e6f);
	//m_horzScale.Recalc();

	m_vertCoord.SetPixelSize(0.2f);
	m_vertCoord.SetPixelSizeLimits(0.001f, 100.f);   
	m_vertCoord.SetZoomFactor(1.3f);
	m_vertCoord.RegisterObserver(this);

	m_hCrsrNS = LoadCursor(NULL, IDC_SIZENS);
	m_hCrsrWE = LoadCursor(NULL, IDC_SIZEWE);
}

void MonitorWindow::Reset()
{
	m_trackStruct.hwndTrack = HWND(0);
	m_pSound->Play(TEXT("DISAPPEAR_SOUND")); 
	(void)TrackMouseEvent(& m_trackStruct);
}

void MonitorWindow::Stop()
{
	Reset();
	m_pNMRI        = nullptr;
	m_pMonitorData = nullptr;
	m_graphics.ShutDown();
	DestroyWindow();
}

LPARAM MonitorWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	//if (m_bShowScale)
	//	AppendMenu(hPopupMenu, MF_STRING, IDD_RULER_OFF, L"Ruler off");
	//else
	//	AppendMenu(hPopupMenu, MF_STRING, IDD_RULER_ON,  L"Ruler on");

	if (m_measurement.IsActive())
		AppendMenu(hPopupMenu, MF_STRING, IDD_MEASUREMENT_OFF, L"Measurement off");
	else
		AppendMenu(hPopupMenu, MF_STRING, IDD_MEASUREMENT_ON,  L"Measurement on");

	if (m_trackNrHighlighted.IsNotNull() && m_pMonitorData->IsEmptyTrack(m_trackNrHighlighted))
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_TRACK, L"Delete track");
	
	if (m_trackNrHighlighted.IsNotNull())
		AppendMenu(hPopupMenu, MF_STRING, IDD_ADD_TRACK, L"Add track");

	if (m_pMonitorData->IsAnySignalSelected())
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_EEG_SENSOR, L"Delete signal");

	return 0L; // will be forwarded to HandleContextMenuCommand
}

fPixel const MonitorWindow::getSignalValue(Signal const & signal, fMicroSecs const time) const
{
	return m_vertCoord.Transform2fPixelSize(signal.GetDataPoint(time));
}

fMicroSecs const MonitorWindow::findNextMax(Signal const & signal, fPixel const fPixX) const
{
	fMicroSecs const usEnd  { m_pNMRI->GetSimulationTime() };
	fPixel     const fTicks { m_fPixWinWidth - fPixX };
	fMicroSecs const usTime { usEnd - m_horzCoord.Transform2logUnitSize(fTicks) };
	fMicroSecs const usMax  { signal.FindNextMaximum(usTime) };
	return usMax;
}

fPixel const MonitorWindow::getSignalOffset(SignalId const & idSignal) const
{ 
	fPixel fPixOffset { calcTrackHeight() * Cast2Float(idSignal.GetTrackNr().GetValue()+1) }; 
	if (m_pMonitorData->IsSelected(idSignal))  
		fPixOffset += Convert2fPixel(m_pixMoveOffsetY);  // may have move offset
	return fPixOffset;
}

void MonitorWindow::highlightSignal(SignalId const & idNew)
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

TrackNr const MonitorWindow::findPos4NewTrack(PIXEL const pixCrsrPosY) const
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

fPixel const MonitorWindow::calcTrackHeight() const
{
	fPixel const fPixRectHeight  { Convert2fPixel(GetClientWindowHeight()) };
	fPixel const fPixExtraSpace  { 10.0_fPixel };
	//fPixel const fPixExtraSpace  { m_bShowScale ? 60.0_fPixel : 10.0_fPixel };
	fPixel const fPixFreeHeight  { fPixRectHeight - fPixExtraSpace };
	fPixel const fPixTrackHeight { 
									  m_pMonitorData->NoTracks() 
									  ? fPixFreeHeight 
									  : fPixFreeHeight / Cast2Float(m_pMonitorData->GetNrOfTracks())
	                             }; 
	return fPixTrackHeight;
}

void MonitorWindow::paintSignal(SignalId const & idSignal) const
{
	Signal const * const pSignal { m_pMonitorData->GetSignalPtr(idSignal) };
	if (pSignal == nullptr)
		return;

	D2D1::ColorF const color        { m_pMonitorData->IsSelected(idSignal) ? NNetColors::EEG_SENSOR_HIGHLIGHTED : D2D1::ColorF::Black };  // emphasize selected signal 
	fPixel       const fPixWidth    { m_pMonitorData->IsSelected(idSignal) ? 3.0_fPixel : 1.0_fPixel };  // emphasize selected signal 
	fPixel       const fPixYoff     { getSignalOffset(idSignal) };
	fMicroSecs   const usInWindow   { m_horzCoord.GetPixelSize() * m_fPixWinWidth.GetValue() };
	fMicroSecs   const usResolution { m_pNMRI->TimeResolution() };
	float        const fPointsInWin { usInWindow / usResolution };
	fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs   const usIncrement  { (fPointsInWin > usPixelSize.GetValue()) ? usPixelSize : usResolution };
	fMicroSecs   const usEnd        { m_pNMRI->GetSimulationTime() };
	fMicroSecs   const timeStart    { max(usEnd - usInWindow, pSignal->GetStartTime()) };
	fPixelPoint        prevPoint    { m_fPixWinWidth, fPixYoff - getSignalValue(*pSignal, usEnd) };

	for (fMicroSecs time = usEnd - usIncrement; time >= timeStart; time -= usIncrement)
	{
		fPixel      const fPixX    { m_fPixWinWidth - m_horzCoord.Transform2fPixelSize(usEnd - time) }; 
		fPixelPoint const actPoint { fPixX, fPixYoff - getSignalValue(*pSignal, time) };
		m_graphics.DrawLine(prevPoint, actPoint, fPixWidth, color);
		prevPoint = actPoint;
	}
}

void MonitorWindow::doPaint() const
{
	if (m_pMonitorData->NoTracks())
		return;

	m_pMonitorData->Apply2AllSignals([&](SignalId const id) { paintSignal(id); });

	//if (m_bShowScale)
	//	m_horzScale.Display();

	if (m_trackNrHighlighted.IsNotNull())  // paint background of selected track
	{
		fPixel         const fPHeight { calcTrackHeight() };
		fPixelPoint    const pos      {	0._fPixel, fPHeight * Cast2Float(m_trackNrHighlighted.GetValue()) };
		fPixelRectSize const size     {	m_fPixWinWidth + 1, fPHeight };
		m_graphics.FillRectangle(fPixelRect(pos, size), NNetColors::COL_BEACON);
	}

	m_measurement.DisplayDynamicScale(fMicroSecs(m_horzCoord.GetPixelSize()));

	if (m_measurement.TrackingActive())
		m_graphics.FillDiamond(calcDiamondPos(), 4.0_fPixel, NNetColors::COL_DIAMOND);
}

fPixelPoint const MonitorWindow::calcDiamondPos() const
{
	SignalId const & idSignal { m_pMonitorData->GetHighlightedSignalId() };
	Signal   const * pSignal  { m_pMonitorData->GetSignalPtr(idSignal) };
	if (!pSignal)
		return fPP_NULL;
	PixelPoint const pixPointCrsr { GetRelativeCrsrPosition() };
	fPixel     const fPixCrsrX    { Convert2fPixel(pixPointCrsr.GetX()) };
	fMicroSecs const usMax        { findNextMax(*pSignal, fPixCrsrX) };
	fMicroSecs const usEnd        { m_pNMRI->GetSimulationTime() };
	fPixel     const fPixMaxX     { m_fPixWinWidth - m_horzCoord.Transform2fPixelSize(usEnd-usMax) };
	fPixel     const fPixYoff     { getSignalOffset(idSignal) };
	fPixel     const fPixYvalue   { fPixYoff - getSignalValue(*pSignal, usMax) };
	return fPixelPoint(fPixMaxX, fPixYvalue);
}

SignalNr const MonitorWindow::findSignal(TrackNr const trackNr, PixelPoint const & ptCrsr) const
{
	SignalNr signalNrRes { SignalNr::NULL_VAL() };

	if (m_pMonitorData->NoTracks())
		return signalNrRes;
	if (trackNr.IsNull())
		return signalNrRes;

	fMicroSecs  const timeStart       { m_horzCoord.Transform2logUnitPos(0.0_fPixel) };
	fPixelPoint const fPixPtCrsr      { Convert2fPixelPoint(ptCrsr) };
	fMicroSecs  const umTime          { m_horzCoord.Transform2logUnitPos(fPixPtCrsr.GetX()) };
	fPixel      const fPixTrackHeight { calcTrackHeight() };
	fPixel      const fPixTrackBottom { fPixTrackHeight * static_cast<float>(trackNr.GetValue() + 1) };  
	fPixel      const fPixCrsrY       { fPixTrackBottom - fPixPtCrsr.GetY() };  // vertical distance from crsr pos to zero line of track
	fPixel            fPixBestDelta   { fPixel::MAX_VAL() };
	m_pMonitorData->Apply2AllSignalsInTrack
	(
		trackNr,
		[&](SignalNr const signalNr)
		{
			Signal const & signal { * m_pMonitorData->GetSignalPtr(SignalId(trackNr, signalNr)) };
			if (umTime >= signal.GetStartTime())
			{
				fPixel const fPixDelta     { fPixCrsrY - getSignalValue(signal, umTime) };
				fPixel const fPixDeltaAbs  { fPixDelta.GetAbs() };
				if (fPixDeltaAbs < fPixBestDelta)
				{
					fPixBestDelta = fPixDeltaAbs;
					signalNrRes = signalNr;
				}
			}
		}
	);
	return (fPixBestDelta <= Convert2fPixel(10_PIXEL)) ? signalNrRes : SignalNr::NULL_VAL();
}

TrackNr const MonitorWindow::findTrack(PIXEL const pixPosY) const
{
	fPixel  const fPixTrackHeight { calcTrackHeight() };
	fPixel  const fPixCrsrYpos    { Convert2fPixel(pixPosY) };
	TrackNr const trackNr         { Cast2Int(fPixCrsrYpos / fPixTrackHeight) };
	return m_pMonitorData->IsValid(trackNr) ? trackNr : TrackNr::NULL_VAL();
}

void MonitorWindow::OnPaint()
{
	if (IsWindowVisible())
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint(&ps);
		if (m_graphics.StartFrame(hDC))
		{
			try
			{
				doPaint();
			}
			catch (MonitorDataException e)
			{
				SendCommand2Application(IDM_STOP, 0);
				MonitorData::HandleException(e);
			}
			m_graphics.EndFrame();
		}
		EndPaint(&ps);
	}
}

bool MonitorWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	int const wmId = LOWORD(wParam);

	switch (wmId)
	{
	case IDD_MEASUREMENT_OFF:
		m_measurement.SetActive(false);
		break;

	case IDD_MEASUREMENT_ON:
		//m_bShowScale = false;
		m_measurement.SetActive(true);
		break;

	//case IDD_RULER_OFF:
	//	m_bShowScale = false;
	//	break;

	//case IDD_RULER_ON:
	//	m_bShowScale = true;
	//	break;

	case IDD_DELETE_EEG_SENSOR:
		PostCommand2Application(wmId, 0);
		break;

	case IDD_ADD_TRACK:
		PostCommand2Application(wmId, static_cast<LPARAM>(findPos4NewTrack(pixPoint.GetY()).GetValue()));
		PostMessage(WM_COMMAND, IDM_WINDOW_ON, 0);  // if window was not visible, show it now
		break;

	case IDD_DELETE_TRACK:
		if (m_pMonitorData->GetNrOfTracks()==1)
			PostMessage(WM_COMMAND, IDM_WINDOW_OFF, 0);
		PostCommand2Application(wmId, static_cast<LPARAM>(m_trackNrHighlighted.GetValue()));
		break;

	default:
		break;
	}

	bool bRes = BaseWindow::OnCommand(wParam, lParam, pixPoint);
	Trigger();

	return bRes; 
}

bool MonitorWindow::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	if (! CrsrInClientRect())
		m_trackNrHighlighted.Set2Null();
	return false;
}

bool MonitorWindow::OnSize(WPARAM const wParam, LPARAM const lParam)
{
	static const float BORDER { 0.1f };
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize(width, height);
	m_fPixWinWidth = Convert2fPixel(PIXEL(width));
	fPixel fPixWinHeight { Convert2fPixel(PIXEL(height)) };
	m_measurement.SetClientRectSize(PIXEL(width), PIXEL(height));
	m_horzCoord.SetOffset(m_fPixWinWidth * 0.1f);
	//m_horzScale.SetOrthoOffset(fPixWinHeight - 20._fPixel);
	//m_horzScale.Recalc();
	return true;
}

bool MonitorWindow::OnShow(WPARAM const wParam, LPARAM const lParam)
{
	if (static_cast<bool>(wParam))
		m_measurement.ResetLimits();
	return false;
}

void MonitorWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam(lParam) };

	if (! m_measurement.TrackingActive())
	{
		TrackNr const trackNrFound { findTrack(pixCrsrPos.GetY()) };
		if (trackNrFound != m_trackNrHighlighted)
		{
			m_trackNrHighlighted = trackNrFound;
			Trigger();   // cause repaint
		}
	}
		
	if (wParam & MK_LBUTTON)
	{
		if (m_pixLast.IsNotNull())
		{
			if (m_measurement.TrackingActive())
			{
				m_measurement.MoveSelection(Convert2fPixel(pixCrsrPos.GetX()));
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
	else  // left button not pressed: select
	{
		SignalNr const signalFound { findSignal(m_trackNrHighlighted, pixCrsrPos) };
		if (signalFound.IsNotNull())
		{
			highlightSignal(SignalId(m_trackNrHighlighted, signalFound));
			SetCursor(m_hCrsrNS);
		}
		else if (m_measurement.Select(Convert2fPixel(pixCrsrPos.GetX())))
		{
			SetCursor(m_hCrsrWE);
		}
	}
	Trigger();   // cause repaint
	(void)TrackMouseEvent(& m_trackStruct);
}

void MonitorWindow::OnLeftButtonDblClick(WPARAM const wParam, LPARAM const lParam) 
{
	fPixelPoint const fPixDiamondPos { calcDiamondPos() };
	if (fPixDiamondPos.IsNull())
		return;
		
	if (! m_pMonitorData->IsAnySignalSelected())
		return;

	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam(lParam) };
	fPixel     const fPixCrsrX  { Convert2fPixel(pixCrsrPos.GetX()) };

	if (m_measurement.IsClose2LeftLimit(fPixCrsrX) || m_measurement.IsClose2RightLimit(fPixCrsrX))
	{
		m_measurement.MoveSelection(fPixDiamondPos.GetX());
		m_pSound->Play(TEXT("SNAP_IN_SOUND")); 
		Trigger();  // cause repaint
	}
};

void MonitorWindow::OnLButtonUp(WPARAM const wParam, LPARAM const lParam) 
{
	if (
		 (m_pMonitorData->GetSelectedTrackNr() != m_trackNrHighlighted) && 
		 (! m_measurement.TrackingActive())                             &&
		 (m_pMonitorData->IsAnySignalSelected())
	   )
	{
		m_pModelCommands->MoveSignal(m_pMonitorData->GetHighlightedSignalId(), m_trackNrHighlighted);
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
	Trigger();  // cause repaint
};

void MonitorWindow::OnMouseWheel(WPARAM const wParam, LPARAM const lParam)
{  
	int  const iDelta     { GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };
	bool       bResult    { true };

	for (int iSteps = abs(iDelta); (iSteps > 0) && bResult; --iSteps)
	{
		bResult = bShiftKey 
			? m_horzCoord.Zoom(bDirection)
			: m_vertCoord.Zoom(bDirection);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}

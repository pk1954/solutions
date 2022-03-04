// MonitorControl.cpp
//
// NNetWindows

#include "stdafx.h"
#include "dwrite.h"
#include "Resource.h"
#include "win32_util_resource.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelCommands.h"
#include "NNetColors.h"
#include "MonitorData.h"
#include "MonitorControl.h"

using std::to_wstring;

MonitorControl::MonitorControl
(
	HWND                     const   hwndParent,
	Sound                          & sound,
	NNetModelCommands              & modelCommands,
	NNetModelReaderInterface const & nmri,
	MonitorData                    & monitorData,
	PixFpDimension<fMicroSecs>     & horzCoord
) :
	m_sound        (sound),
	m_modelCommands(modelCommands),
	m_nmri         (nmri),
	m_monitorData  (monitorData),
	m_horzCoord    (horzCoord)
{
	GraphicsWindow::Initialize(hwndParent, L"ClassMonitorControl", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
	m_measurement.Initialize(m_upGraphics.get());
	m_pTextFormat = m_upGraphics->NewTextFormat(16.f);
	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	m_hCrsrNS = LoadCursor(nullptr, IDC_SIZENS);
	m_hCrsrWE = LoadCursor(nullptr, IDC_SIZEWE);

	m_vertCoord.SetPixelSize(0.2f);
	m_vertCoord.SetPixelSizeLimits(0.001f, 100.f);   
	m_vertCoord.SetZoomFactor(1.3f);

	m_horzCoord.RegisterObserver(*this);
	m_vertCoord.RegisterObserver(*this);
}

void MonitorControl::Reset()
{
	m_sound.Play(TEXT("DISAPPEAR_SOUND"));
	GraphicsWindow::Reset();
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

	if (m_trackNrHighlighted.IsNotNull() && m_monitorData.IsEmptyTrack(m_trackNrHighlighted))
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_TRACK, L"Delete track");

	if (m_trackNrHighlighted.IsNotNull())
		AppendMenu(hPopupMenu, MF_STRING, IDD_ADD_TRACK, L"Add track");

	if (m_monitorData.IsAnySignalSelected())
		AppendMenu(hPopupMenu, MF_STRING, IDD_DELETE_EEG_SENSOR, L"Delete signal");

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

SignalNr MonitorControl::findSignal(TrackNr const trackNr, PixelPoint const & ptCrsr) const
{
	SignalNr signalNrRes { SignalNr::NULL_VAL() };

	if (m_monitorData.NoTracks() || trackNr.IsNull())
		return signalNrRes;

	fPixelPoint const fPixPtCrsr    { Convert2fPixelPoint(ptCrsr) };
	fMicroSecs  const usTime        { m_horzCoord.Transform2logUnitPos(fPixPtCrsr.GetX()) };
	fPixel            fPixBestDelta { fPixel::MAX_VAL() };
	m_monitorData.Apply2AllSignalsInTrackC
	(
		trackNr,
		[this, &trackNr, &usTime, &fPixPtCrsr, &fPixBestDelta, &signalNrRes](SignalNr const signalNr)
		{
			SignalId idSignal { SignalId(trackNr, signalNr) };
			if (Signal const * pSignal { m_monitorData.GetConstSignalPtr(idSignal) })
			{
				fPixel const fPixSignal { getSignalValue(*pSignal, usTime) };
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
//	return (fPixBestDelta <= Convert2fPixel(10_PIXEL)) ? signalNrRes : SignalNr::NULL_VAL();
}

TrackNr MonitorControl::findTrack(PIXEL const pixPosY) const
{
	fPixel  const fPixTrackHeight { calcTrackHeight() };
	fPixel  const fPixCrsrYpos    { Convert2fPixel(pixPosY) };
	TrackNr const trackNr         { Cast2Int(fPixCrsrYpos / fPixTrackHeight) };
	return m_monitorData.IsValid(trackNr) ? trackNr : TrackNr::NULL_VAL();
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
	if (! m_monitorData.IsSelected(idNew))
	{
		if (m_monitorData.IsValid(idNew))
		{
			m_monitorData.SetHighlightedSignal(idNew);
		}
		else
		{
			m_monitorData.ResetHighlightedSignal();
			m_pixLast.Set2Null();
		}
		m_pixMoveOffsetY = 0_PIXEL;
	}
}

fPixel MonitorControl::getSignalOffset(SignalId const & idSignal) const
{ 
	fPixel fPixOffset { calcTrackHeight() * Cast2Float(idSignal.GetTrackNr().GetValue()+1) }; 
	if (m_monitorData.IsSelected(idSignal))  
		fPixOffset += Convert2fPixel(m_pixMoveOffsetY);  // may have move offset
	return fPixOffset;
}

fPixel MonitorControl::getSignalValue(Signal const & signal, fMicroSecs const usSimu) const
{
	float const fSignal { signal.GetDataPoint(usSimu) };
	return (fSignal == NAN)
		? fPixel::NULL_VAL()
		: m_vertCoord.Transform2fPixelSize(fSignal) * m_fScaleFactor;
}

fPixel MonitorControl::calcTrackHeight() const
{
	fPixel const fPixRectHeight  { Convert2fPixel(GetClientWindowHeight()) };
	fPixel const fPixTrackHeight { 
		m_monitorData.NoTracks() 
		? fPixRectHeight 
		: fPixRectHeight / Cast2Float(m_monitorData.GetNrOfTracks())
	}; 
	return fPixTrackHeight;
}

fPixelPoint MonitorControl::calcDiamondPos() const
{
	fPixelPoint        fPixDiamondPos { fPP_NULL };
	SignalId   const & idSignal       { m_monitorData.GetHighlightedSignalId() };
	if (Signal const * pSignal        { m_monitorData.GetConstSignalPtr(idSignal) })
	{
		fMicroSecs const usSimuEnd  { m_nmri.GetSimulationTime() };
		fPixel     const fPixSignal { getSignalValue(*pSignal, usSimuEnd) };
		if (fPixSignal.IsNotNull())
		{
			PixelPoint const pixPointCrsr { GetRelativeCrsrPosition() };
			fPixel     const fPixCrsrX    { Convert2fPixel(pixPointCrsr.GetX()) };
			fMicroSecs const usScaleTime  { m_horzCoord.Transform2logUnitPos(fPixCrsrX) };
			fMicroSecs const usAbsTime    { usSimuEnd + usScaleTime };
			fMicroSecs const usAbsMax     { pSignal->FindNextMaximum(usAbsTime) };

			fMicroSecs const usScaleMax   { usAbsMax - usSimuEnd };
			fPixel     const fPixMaxX     { m_fPixZeroX + m_horzCoord.Transform2fPixelSize(usScaleMax) };

			fPixel     const fPixYoff     { getSignalOffset(idSignal) };
			fPixel     const fPixYpos     { fPixYoff - getSignalValue(*pSignal, usAbsMax) };
			fPixDiamondPos = fPixelPoint(fPixMaxX, fPixYpos);
		}
	}
	return fPixDiamondPos;
}

void MonitorControl::paintSignal(SignalId const & idSignal)
{
	Signal    const * const pSignal { m_monitorData.GetConstSignalPtr(idSignal) };
	if (pSignal == nullptr)
		return;
	fMicroSecs     const usSimuEnd  { m_nmri.GetSimulationTime() };
	fPixel               fPixSignal { getSignalValue(*pSignal, usSimuEnd) };
	if (fPixSignal.IsNull())
		return;

	D2D1::ColorF const color        { m_monitorData.IsSelected(idSignal) ? NNetColors::EEG_SENSOR_HIGHLIGHTED : D2D1::ColorF::Black };  // emphasize selected signal 
	fPixel       const fPixWidth    { m_monitorData.IsSelected(idSignal) ? 3.0_fPixel : 1.0_fPixel };  // emphasize selected signal 
	fPixel       const fPixYoff     { getSignalOffset(idSignal) };
	fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
	fMicroSecs   const usInWindow   { usPixelSize * m_fPixZeroX.GetValue() };
	fMicroSecs   const usResolution { m_nmri.TimeResolution() };
	fMicroSecs   const usIncrement  { max(usPixelSize, usResolution) };
	fMicroSecs   const usSimuStart  { max(usSimuEnd - usInWindow, pSignal->GetStartTime()) };
	fPixel       const fPixX        { m_fPixZeroX - m_horzCoord.Transform2fPixelSize(usSimuEnd) };
	fPixelPoint        prevPoint    { m_fPixZeroX, fPixYoff - fPixSignal };
	m_upGraphics->FillCircle(fPixelCircle(prevPoint, 4.0_fPixel), color);
	for (fMicroSecs usSimu = usSimuEnd - usIncrement; usSimu >= usSimuStart; usSimu -= usIncrement)
	{
		fPixSignal = getSignalValue(*pSignal, usSimu);
		if (fPixSignal > m_fPixMaxSignal)
			m_fPixMaxSignal = fPixSignal;
		fPixelPoint const actPoint 
		{ 
			fPixX + m_horzCoord.Transform2fPixelSize(usSimu), 
			fPixYoff - fPixSignal
		};
		m_upGraphics->DrawLine(prevPoint, actPoint, fPixWidth, color);
		prevPoint = actPoint;
	}
}

bool MonitorControl::SignalTooHigh() const
{
	fPixel const fPixTrackHeight { calcTrackHeight() };
	return m_fPixMaxSignal > fPixTrackHeight;
}

void MonitorControl::ScaleSignals()
{
	if (m_fPixMaxSignal > 0.0_fPixel) 
		m_fScaleFactor *= calcTrackHeight() / m_fPixMaxSignal;
}

void MonitorControl::paintTrack(TrackNr const trackNr) const
{
	ColorF const col 
	{
		(trackNr == m_trackNrHighlighted) 
		? NNetColors::COL_BEACON
		: trackNr.GetValue()%2 ? NNetColors::COL_TRACK_ODD : NNetColors::COL_TRACK_EVEN
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

	fPixel const TRACK_NR_WIDTH  { 15._fPixel };
	fPixel const TRACK_NR_HEIGHT { 30._fPixel };
	m_upGraphics->DisplayText
	(
		fPixelRect
		(
			fPixelPoint   (m_fPixWinWidth - TRACK_NR_WIDTH, fPixTrackTop), 
			fPixelRectSize(TRACK_NR_WIDTH, TRACK_NR_HEIGHT)
		),
		to_wstring(trackNr.GetValue()), 
		D2D1::ColorF::Black, 
		m_pTextFormat
	);
}

void MonitorControl::DoPaint()
{
	try
	{
		m_fPixMaxSignal = 0.0_fPixel;

		if (m_monitorData.NoTracks())
			return;

		m_monitorData.Apply2AllTracksC([this](TrackNr const trackNr) { paintTrack(trackNr); });

		m_monitorData.Apply2AllSignalIdsC([this](SignalId const id) { paintSignal(id); });

		m_measurement.DisplayDynamicScale(fMicroSecs(m_horzCoord.GetPixelSize()));

		if (m_measurement.TrackingActive())
		{
			fPixelPoint const fPixDiamondPos { calcDiamondPos() };
			if (fPixDiamondPos.IsNotNull())
				m_upGraphics->FillDiamond(fPixDiamondPos, 4.0_fPixel, NNetColors::COL_DIAMOND);
		}

		if ( SignalTooHigh() )
		{
			m_upGraphics->FillRectangle
			(
				fPixelRect
				(
					fPixelPoint   (m_fPixZeroX, 0._fPixel),
					fPixelRectSize(m_fPixRightBorder, Convert2fPixel(GetClientWindowHeight()))
				), 
				NNetColors::COL_WARNING
			);
		}
	}
	catch (MonitorDataException const & e)
	{
		SendCommand2Application(IDM_STOP, 0);
		MonitorData::HandleException(e);
	}
}

bool MonitorControl::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (int const wmId = LOWORD(wParam))
	{
	case IDD_MEASUREMENT_OFF:
		m_measurement.SetActive(false);
		break;

	case IDD_MEASUREMENT_ON:
		m_measurement.SetActive(true);
		break;

	case IDD_DELETE_EEG_SENSOR:
		PostCommand2Application(wmId, 0);
		break;

	case IDD_ADD_TRACK:
		PostCommand2Application(wmId, static_cast<LPARAM>(findPos4NewTrack(pixPoint.GetY()).GetValue()));
		PostMessage(WM_COMMAND, IDM_WINDOW_ON, 0);  // if window was not visible, show it now
		break;

	case IDD_DELETE_TRACK:
		if (m_monitorData.GetNrOfTracks()==1)
			PostMessage(WM_COMMAND, IDM_WINDOW_OFF, 0);
		PostCommand2Application(wmId, static_cast<LPARAM>(m_trackNrHighlighted.GetValue()));
		break;

	case IDD_SCALE_EEG_SIGNALS:
		ScaleSignals();
		break;

	default:
		break;
	}

	bool bRes = BaseWindow::OnCommand(wParam, lParam, pixPoint);
	Trigger();

	return bRes; 
}

bool MonitorControl::OnMouseLeave(WPARAM const wParam, LPARAM const lParam)
{
	if (! CrsrInClientRect())
		m_trackNrHighlighted.Set2Null();
	return false;
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
			if (fPixNewPos > m_fPixZeroX)
				fPixNewPos = m_fPixZeroX;
			m_measurement.MoveSelection(fPixNewPos);
			SetCursor(m_hCrsrWE);
		}
		else 
		{
			if (m_monitorData.IsAnySignalSelected())
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
	TrackMouse();
}

void MonitorControl::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam) 
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam(lParam) };
	fPixel     const fPixCrsrX  { Convert2fPixel(pixCrsrPos.GetX()) };

	if (fPixCrsrX > m_fPixZeroX)
	{
		if (SignalTooHigh())
			ScaleSignals();
	}
	else
	{
		fPixelPoint const fPixDiamondPos { calcDiamondPos() };
		if (
			  fPixDiamondPos.IsNotNull() && m_monitorData.IsAnySignalSelected() &&
              (m_measurement.IsClose2LeftLimit(fPixCrsrX) || m_measurement.IsClose2RightLimit(fPixCrsrX))
	       )
		{
			m_measurement.MoveSelection(fPixDiamondPos.GetX());
			m_sound.Play(TEXT("SNAP_IN_SOUND")); 
			Trigger();  // cause repaint
		}
	}
};

void MonitorControl::OnLButtonUp(WPARAM const wParam, LPARAM const lParam) 
{
	if (
		  (m_monitorData.GetSelectedTrackNr() != m_trackNrHighlighted) && 
		  (! m_measurement.TrackingActive())                           &&
		  (m_monitorData.IsAnySignalSelected())
	   )
	{
		m_modelCommands.MoveSignal(m_monitorData.GetHighlightedSignalId(), m_trackNrHighlighted);
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
	Trigger();  // cause repaint
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
			? m_horzCoord.Zoom(bDirection)
			: m_vertCoord.Zoom(bDirection);
	}
	if (!bResult)
		MessageBeep(MB_ICONWARNING);
}

bool MonitorControl::OnSize(PIXEL const width, PIXEL const height)
{
	GraphicsWindow::OnSize(width, height);
	m_fPixWinWidth = Convert2fPixel(width);
	m_fPixZeroX    = Convert2fPixel(width) - m_fPixRightBorder;
	m_measurement.SetClientRectSize(width, height);

	return true;
}

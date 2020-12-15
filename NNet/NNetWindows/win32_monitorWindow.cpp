// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include <assert.h>
#include "util.h"
#include "scale.h"
#include "SignalInterface.h"
#include "Track.h"
#include "Resource.h"
#include "BaseKnot.h"
#include "BeaconAnimation.h"
#include "MonitorData.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "win32_monitorWindow.h"

using std::find;

void MonitorWindow::Start
( 
	HWND                     const   hwndParent,
	Sound                  * const   pSound,
	NNetController         * const   pController,
	NNetModelReaderInterface const & model,
	Param                    const & params,
	MonitorData                    & monitorData  
)
{
	HWND hwnd = StartBaseWindow
	( 
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		L"ClassMonitorWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
	m_pSound           =   pSound;
	m_pController      =   pController;
	m_pParams          = & params;
	m_pMRI             = & model;
	m_pMonitorData     = & monitorData;
	m_graphics.Initialize( hwnd );
	SetWindowText( hwnd, L"Monitor" );
	m_trackStruct.hwndTrack = hwnd;
	m_measurement.Initialize( & m_graphics );
	m_scale.Initialize( & m_graphics, L"s" );
	m_scale.SetHorzPixelSize( m_fMicroSecsPerPixel.GetValue() );
	m_hCrsrNS = LoadCursor( NULL, IDC_SIZENS );
	m_hCrsrWE = LoadCursor( NULL, IDC_SIZEWE );
}

void MonitorWindow::Reset( )
{
	deselectSignal( );
	m_trackStruct.hwndTrack = HWND(0);
	(void)TrackMouseEvent( & m_trackStruct );
}

void MonitorWindow::Stop( )
{
	Reset( );
	m_pParams          = nullptr;
	m_pMRI             = nullptr;
	m_pMonitorData     = nullptr;
	m_graphics.ShutDown( );
	DestroyWindow( );
}

long MonitorWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_bShowScale )
		AppendMenu( hPopupMenu, MF_STRING, IDD_RULER_OFF, L"Ruler off" );
	else
		AppendMenu( hPopupMenu, MF_STRING, IDD_RULER_ON,  L"Ruler on" );

	if ( m_measurement.IsActive() )
		AppendMenu( hPopupMenu, MF_STRING, IDD_MEASUREMENT_OFF, L"Measurement off" );
	else
		AppendMenu( hPopupMenu, MF_STRING, IDD_MEASUREMENT_ON,  L"Measurement on" );

	AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_TRACK, L"Delete track" );
	AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_TRACK,    L"Add track" );

	if ( m_idSigSelected.GetSignalNr().IsNotNull() )
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL, L"Delete signal" );

	return 0L; // will be forwarded to HandleContextMenuCommand
}

void MonitorWindow::AddSignal( ShapeId const id )
{
	if ( m_pMRI->IsOfType<Neuron>( id ) )
	{
		m_pMonitorData->InsertTrack( TrackNr(0) );
		m_pMonitorData->AddSignal( TrackNr(0), id );
		m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
	}
}

void MonitorWindow::AddSignal( MicroMeterCircle const & umCircle )
{
	m_pMonitorData->InsertTrack( TrackNr(0) );
	m_pMonitorData->AddSignal( TrackNr(0), umCircle );
	m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
}

void MonitorWindow::InsertTrack( TrackNr const trackNr )
{
	m_pMonitorData->InsertTrack( trackNr );
	m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
}

void MonitorWindow::selectSignal( SignalId const & idNew )
{
	if ( idNew != m_idSigSelected )
	{
		m_pMonitorData->Animation( m_idSigSelected, false );

		if ( m_pMonitorData->IsValid( idNew ) )
		{
			m_pMonitorData->Animation( idNew, true );
			m_idSigSelected = idNew;
		}
		else
		{
			m_idSigSelected.Set2Null();
			m_pixLast.Set2Null();
		}

		m_pixMoveOffsetY = 0_PIXEL;
		Trigger( );   // cause repaint
	}
}

void MonitorWindow::deselectSignal( )
{
	selectSignal( SignalIdNull );
	m_pSound->Play( TEXT("DISAPPEAR_SOUND") ); 
}

TrackNr const MonitorWindow::findPos4NewTrack( PIXEL const pixCrsrPosY ) const
{
	fPIXEL const fPixTrackHeight  { calcTrackHeight() };
	fPIXEL const fPixCrsrYpos     { Convert2fPIXEL( pixCrsrPosY ) };
	int    const iTrackNr         { Cast2Int( fPixCrsrYpos / fPixTrackHeight ) };
	fPIXEL const fPixTrackCenterY { fPixTrackHeight * (Cast2Float(iTrackNr) + 0.5f) };
	TrackNr      trackNr          { m_idSigSelected.GetTrackNr() };
	if ( fPixCrsrYpos > fPixTrackCenterY )
		++trackNr;
	return trackNr;
}

fPIXEL const MonitorWindow::calcTrackHeight( ) const
{
	PIXEL const pixRectHeight  { GetClientWindowHeight( ) };
	PIXEL const pixExtraSpace  { m_bShowScale ? 60_PIXEL : 10_PIXEL };
	PIXEL const pixFreeHeight  { pixRectHeight - pixExtraSpace };
	PIXEL const pixTrackHeight { 
		                          m_pMonitorData->NoTracks() 
		                          ? pixFreeHeight 
		                          : pixFreeHeight / Cast2Long(m_pMonitorData->GetNrOfTracks()) 
	                           }; 
	return Convert2fPIXEL( pixTrackHeight );
}

void MonitorWindow::paintSignal( SignalId const & idSignal ) const
{
	fPIXEL          const   fPixWidth    { (idSignal == m_idSigSelected) ? (m_bSignalLocked ? 3.0_fPIXEL : 2.0_fPIXEL) : 1.0_fPIXEL };  // emphasize selected signal 
	fPIXEL          const   fPixYoff     { getSignalOffset( idSignal ) };
	fMicroSecs      const   usInWindow   { m_fMicroSecsPerPixel * m_fPixWinWidth.GetValue() };
	fMicroSecs      const   usResolution { m_pParams->GetTimeResolution( ) };
	float           const   fPointsInWin { usInWindow / usResolution };
	fMicroSecs      const   usIncrement  { (fPointsInWin > m_fPixWinWidth.GetValue()) ? m_fMicroSecsPerPixel : usResolution };
	fMicroSecs      const   usEnd        { m_pMRI->GetSimulationTime( ) };
	SignalInterface const & signal       { m_pMonitorData->GetSignal( idSignal ) }; 
	fMicroSecs      const   timeStart    { max( usEnd - usInWindow, signal.GetStartTime() ) };
	fPixelPoint             prevPoint    { m_fPixWinWidth, fPixYoff - getYvalue(signal, usEnd) };
	fPIXEL                  fPixYlimit   { 0._fPIXEL };

	if ( signal.MarkLineAboveThreshold() )
	{
		mV const mVlimit ( m_pParams->GetParameterValue( tParameter::threshold ) );
		fPixYlimit = fPixYoff - yValue2fPIXEL(mVlimit.GetValue());
	}

	for ( fMicroSecs time = usEnd - usIncrement; time >= timeStart; time -= usIncrement )
	{
		fPixelPoint const actPoint 
		{ 
			fMicroSecs2fPIXEL( time ), 
			fPixYoff - getYvalue( signal, time ) 
		};

		if ( (prevPoint.GetY() >= fPixYlimit) && (actPoint.GetY() >= fPixYlimit) )  // line is completely below threshold
		{
			m_graphics.DrawLine( prevPoint, actPoint, fPixWidth, NNetColors::COL_BLACK );
		}
		else if ( (prevPoint.GetY() < fPixYlimit) && (actPoint.GetY() < fPixYlimit) )  // line is completely above threshold
		{
			m_graphics.DrawLine( prevPoint, actPoint, fPixWidth, NNetColors::COL_RED );
		}
		else // part of line is below threshhold. Interpolate at threshold and split into two lines
		{
			fPixelPoint fPixDelta  { actPoint - prevPoint };
			float       fSlope     { fPixDelta.GetX() / fPixDelta.GetY() };
			fPIXEL      fPixXlimit { (fPixYlimit - prevPoint.GetY()) * fSlope + prevPoint.GetX() };
			fPixelPoint fPixLimit  { fPixXlimit, fPixYlimit };
			if ( actPoint.GetY() < prevPoint.GetY() )
			{
				m_graphics.DrawLine( prevPoint, fPixLimit, fPixWidth, NNetColors::COL_BLACK );
				m_graphics.DrawLine( fPixLimit, actPoint,  fPixWidth, NNetColors::COL_RED );
			}
			else
			{
				m_graphics.DrawLine( prevPoint, fPixLimit, fPixWidth, NNetColors::COL_RED );
				m_graphics.DrawLine( fPixLimit, actPoint,  fPixWidth, NNetColors::COL_BLACK );
			}
		}

		prevPoint = actPoint;
	}
}

void MonitorWindow::doPaint( ) const
{
	if ( m_pMonitorData->NoTracks() )
		return;

	m_pMonitorData->Apply2AllSignals( [&]( SignalId const id ) { paintSignal( id ); } );

	if ( m_bShowScale )
		m_scale.DisplayStaticScale( );

	if ( m_trackNrHighlighted.IsNotNull() )  // paint background of selected track
	{
		fPIXEL         const fPHeight { calcTrackHeight() };
		fPixelPoint    const pos      {	0._fPIXEL, fPHeight * Cast2Float(m_trackNrHighlighted.GetValue()) };
		fPixelRectSize const size     {	m_fPixWinWidth, fPHeight };
		m_graphics.FillRectangle( fPixelRect( pos, size ), NNetColors::COL_BEACON );
	}

	m_measurement.DisplayDynamicScale( m_fMicroSecsPerPixel );

	if ( m_idSigSelected != SignalIdNull )
		drawDiamond( m_idSigSelected );
}

void MonitorWindow::drawDiamond( SignalId const & idSignal ) const
{
	static D2D1::ColorF const COL_DIAMOND { 0.0f, 1.0f, 0.0f, 1.0f };

	SignalInterface const & signal       { m_pMonitorData->GetSignal( idSignal ) };
	PixelPoint      const   pixPointCrsr { GetRelativeCrsrPosition( ) };
	fPIXEL          const   fPixCrsrX    { Convert2fPIXEL( pixPointCrsr.GetX() ) };
	fMicroSecs      const   usMax        { findNextMax( signal, fPixCrsrX ) };
	fPIXEL          const   fPixMax      { fMicroSecs2fPIXEL( usMax ) };
	fPIXEL          const   fPixYoff     { getSignalOffset( idSignal ) };
	fPIXEL          const   fPixYvalue   { fPixYoff - getYvalue( signal, usMax ) };

	m_graphics.FillDiamond
	(
		fPixelPoint( fPixMax, fPixYvalue ),
		4.0_fPIXEL,
		COL_DIAMOND
	);
}

SignalNr const MonitorWindow::findSignal( TrackNr const trackNr, PixelPoint const & ptCrsr ) const
{
	SignalNr signalNrRes { SignalNr::NULL_VAL() };

	if ( m_bSignalLocked )
		return signalNrRes;
	if ( m_pMonitorData->NoTracks() )
		return signalNrRes;
	if ( trackNr.IsNull() )
		return signalNrRes;

	fMicroSecs  const timeStart       { fPIXEL2fMicroSecs( 0.0_fPIXEL ) };
	fPixelPoint const fPixPtCrsr      { Convert2fPixelPoint( ptCrsr ) };
	fMicroSecs  const umTime          { fPIXEL2fMicroSecs( fPixPtCrsr.GetX() ) };
	fPIXEL      const fPixTrackHeight { calcTrackHeight() };
	fPIXEL      const fPixTrackBottom { fPixTrackHeight * static_cast<float>(trackNr.GetValue() + 1) };  
	fPIXEL      const fPixCrsrY       { fPixTrackBottom - fPixPtCrsr.GetY() };  // vertical distance from crsr pos to zero line of track
	fPIXEL            fPixBestDelta   { fPIXEL::MAX_VAL() };
	m_pMonitorData->Apply2AllSignalsInTrack
	(
		trackNr,
		[&](SignalNr const signalNr)
		{
			SignalInterface const & signal { m_pMonitorData->GetSignal( SignalId(trackNr, signalNr) ) };
			if ( umTime >= signal.GetStartTime() )
			{
				fPIXEL const fPixAmplitude { getYvalue( signal, umTime ) };
				fPIXEL const fPixDelta     { fPixAmplitude - fPixCrsrY };
				fPIXEL const fPixDeltaAbs  { fPixDelta.GetAbs( ) };
				if ( fPixDeltaAbs < fPixBestDelta )
				{
					fPixBestDelta = fPixDeltaAbs;
					signalNrRes = signalNr;
				}
			}
		}
	);
	return (fPixBestDelta <= Convert2fPIXEL(10_PIXEL)) ? signalNrRes : SignalNr::NULL_VAL();
}

TrackNr const MonitorWindow::findTrack( PIXEL const pixPosY ) const
{
	fPIXEL  const fPixTrackHeight { calcTrackHeight() };
	fPIXEL  const fPixCrsrYpos    { Convert2fPIXEL( pixPosY ) };
	TrackNr const trackNr         { Cast2Int( fPixCrsrYpos / fPixTrackHeight ) };
	return m_pMonitorData->IsValid( trackNr ) ? trackNr : TrackNr::NULL_VAL();
}

void MonitorWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_graphics.StartFrame( hDC ) )
		{
			doPaint( );
			m_graphics.EndFrame( );
		}
		EndPaint( &ps );
	}
}

bool MonitorWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	int const wmId = LOWORD( wParam );

	switch (wmId)
	{
	case IDD_MEASUREMENT_OFF:
		m_measurement.SetActive( false );
		break;

	case IDD_MEASUREMENT_ON:
		m_bShowScale = false;
		m_measurement.SetActive( true );
		break;

	case IDD_RULER_OFF:
		m_bShowScale = false;
		break;

	case IDD_RULER_ON:
		m_bShowScale = true;
		break;

	case IDD_DELETE_SIGNAL:
		m_pMonitorData->DeleteSignal( m_idSigSelected );
		deselectSignal( );
		break;

	case IDD_ADD_TRACK:
		InsertTrack( findPos4NewTrack( pixPoint.GetY() ) );
		Show( true );            // if window was not visible, show it now
		break;

	case IDD_DELETE_TRACK:
		m_pMonitorData->DeleteTrack( m_trackNrHighlighted );
		deselectSignal( );
		if ( m_pMonitorData->NoTracks() )
			SendMessage( WM_COMMAND, IDM_WINDOW_OFF, 0 );
		break;

	default:
		break;
	}

	bool bRes = BaseWindow::OnCommand( wParam, lParam, pixPoint );
	Trigger( );

	return bRes; 
}

bool MonitorWindow::OnMouseLeave( WPARAM const wParam, LPARAM const lParam )
{
	if ( ! CrsrInClientRect() )
		m_trackNrHighlighted.Set2Null();
	return false;
}

bool MonitorWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize( width, height );
	m_scale      .SetClientRectSize( PIXEL(width), PIXEL(height) );
	m_measurement.SetClientRectSize( PIXEL(width), PIXEL(height) );
	m_fPixWinWidth = Convert2fPIXEL( PIXEL(width) );
	return true;
}

bool MonitorWindow::OnShow( WPARAM const wParam, LPARAM const lParam )
{
	bool bShow { static_cast<bool>(wParam) };
	if ( bShow )
	{
		m_measurement.ResetLimits( );
		if ( m_idSigSelected != SignalIdNull )
			m_pMonitorData->Animation( m_idSigSelected, true );
	}
	else 
		m_pMonitorData->Animation( m_idSigSelected, false );
	return false;
}

void MonitorWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam( lParam ) };

	if ( ! m_measurement.TrackingActive() )
	{
		TrackNr const trackNrFound { findTrack( pixCrsrPos.GetY() ) };
		if ( trackNrFound != m_trackNrHighlighted )
		{
			m_trackNrHighlighted = trackNrFound;
			Trigger( );   // cause repaint
		}
	}
		
	if ( wParam & MK_LBUTTON )
	{
		if ( m_pixLast.IsNotNull( ) )
		{
			if ( m_measurement.TrackingActive() )
			{
				m_measurement.MoveSelection( Convert2fPIXEL( pixCrsrPos.GetX() ) );
				SetCursor( m_hCrsrWE );
			}
			else 
			{
				if ( m_idSigSelected.GetSignalNr().IsNotNull() && ! m_bSignalLocked )
				{
					m_pixMoveOffsetY += pixCrsrPos.GetY() - m_pixLast.GetY();
					SetCursor( m_hCrsrNS );
				}
			}
		}
		m_pixLast = pixCrsrPos;
		Trigger( );   // cause repaint
	}
	else  // left button not pressed: select
	{
		SignalNr const signalFound { findSignal( m_trackNrHighlighted, pixCrsrPos ) };
		if ( signalFound.IsNotNull() )
		{
			selectSignal( SignalId( m_trackNrHighlighted, signalFound ) );
			SetCursor( m_hCrsrNS );
		}
		else if ( m_measurement.Select( Convert2fPIXEL( pixCrsrPos.GetX() ) ) )
		{
			SetCursor( m_hCrsrWE );
			Trigger( );
		}

	}
	(void)TrackMouseEvent( & m_trackStruct );
}

void MonitorWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam ) 
{
	if ( m_idSigSelected == SignalIdNull )
		return;

	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam( lParam ) };
	fPIXEL     const fPixCrsrX  { Convert2fPIXEL( pixCrsrPos.GetX() ) };

	if ( m_measurement.IsClose2LeftLimit( fPixCrsrX ) || m_measurement.IsClose2RightLimit( fPixCrsrX ) )
	{
		SignalInterface     const & signal  { m_pMonitorData->GetSignal( m_idSigSelected ) };
		fMicroSecs const   usMax   { findNextMax( signal, fPixCrsrX ) };
		fPIXEL     const   fPixMax { fMicroSecs2fPIXEL( usMax ) };
		m_measurement.MoveSelection( fPixMax );
		m_pSound->Play( TEXT("SNAP_IN_SOUND") ); 
		Trigger();  // cause repaint
	}
	else
	{
		m_bSignalLocked = ! m_bSignalLocked;
		m_pSound->Play( m_bSignalLocked ? TEXT("SNAP_IN_SOUND") : TEXT("UNLOCK_SOUND") ); 
	}
};

void MonitorWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam ) 
{
	if ( 
		 (m_idSigSelected.GetTrackNr() != m_trackNrHighlighted) && 
		 (! m_measurement.TrackingActive()) && 
		 (! m_bSignalLocked) 
	   )
	{
		m_idSigSelected = m_pMonitorData->MoveSignal( m_idSigSelected, m_trackNrHighlighted );
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
	Trigger();  // cause repaint
};

void MonitorWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{  
	static float const ZOOM_FACTOR { 1.3f };

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA };
	bool const bShiftKey  { (wParam & MK_SHIFT) != 0 };
	bool const bDirection { iDelta > 0 };

	for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
	{
		if ( bShiftKey )
		{
			static const fMicroSecs LOWER_LIMIT {   1.0_MicroSecs };
			static const fMicroSecs UPPER_LIMIT { 400.0_MicroSecs };
			if ( bDirection )
			{
				if ( m_fMicroSecsPerPixel > LOWER_LIMIT )
					m_fMicroSecsPerPixel /= ZOOM_FACTOR;
				else 
					MessageBeep( MB_ICONWARNING );
			}
			else
			{
				if ( m_fMicroSecsPerPixel < UPPER_LIMIT )
					m_fMicroSecsPerPixel *= ZOOM_FACTOR;
				else 
					MessageBeep( MB_ICONWARNING );
			}
			m_scale.SetHorzPixelSize( m_fMicroSecsPerPixel.GetValue() );
		}
		else
		{
			static const float LOWER_LIMIT { 0.001f };
			static const float UPPER_LIMIT { 100.0f };
			if ( bDirection )
			{
				if ( m_fYvaluesPerPixel > LOWER_LIMIT )
					m_fYvaluesPerPixel /= ZOOM_FACTOR;
				else 
					MessageBeep( MB_ICONWARNING );
			}
			else
			{
				if ( m_fYvaluesPerPixel < UPPER_LIMIT )
					m_fYvaluesPerPixel *= ZOOM_FACTOR;
				else 
					MessageBeep( MB_ICONWARNING );
			}
		}
	}

	Trigger();  // cause repaint
}

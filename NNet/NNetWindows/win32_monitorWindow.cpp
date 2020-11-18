// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include <assert.h>
#include "util.h"
#include "scale.h"
#include "Signal.h"
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
	NNetController         * const   pController,
	NNetModelReaderInterface const & model,
	Param                    const & params,
	BeaconAnimation                & beaconAnimation,
	MonitorData                    & monitorData  
)
{
	HWND hwnd = StartBaseWindow
	( 
		hwndParent,
		CS_OWNDC | CS_HREDRAW | CS_VREDRAW, 
		L"ClassMonitorWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
	m_pController      =   pController;
	m_pBeaconAnimation = & beaconAnimation;
	m_pParams          = & params;
	m_pMRI             = & model;
	m_pMonitorData     = & monitorData;
	m_graphics.Initialize( hwnd );
	SetWindowText( hwnd, L"Monitor" );
	m_trackStruct.hwndTrack = hwnd;
	m_measurement.Initialize( & m_graphics );
	m_scale.Initialize( & m_graphics, L"s" );
	m_scale.SetHorzPixelSize( m_fMicroSecsPerPixel.GetValue() );
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
	m_pBeaconAnimation = nullptr;
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
		m_pMonitorData->AddSignal( id, TrackNr(0) );
	}
}

void MonitorWindow::InsertTrack( TrackNr const trackNr )
{
	m_pMonitorData->InsertTrack( trackNr );
}

void MonitorWindow::selectSignal( SignalId const & idNew )
{
	if ( idNew != m_idSigSelected )
	{
		m_pBeaconAnimation->Stop( );
		 
		if ( m_pMonitorData->IsValid( idNew ) )
		{
			m_pBeaconAnimation->Start( m_pMonitorData->GetSignal( idNew ).GetSignalSource() );
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

fPIXEL const MonitorWindow::getYvalue( Signal const & signal, fMicroSecs const time ) const
{
	float  const fDataPoint { signal.GetDataPoint( time ) };
	fPIXEL const fPixYvalue { fDataPoint / m_fYvaluesPerPixel };
	return fPixYvalue;
}

void MonitorWindow::paintSignal
( 
	SignalId   const & idSignal, 
	fPIXEL     const   fPixTrackHeight,
	fPIXEL     const   fPixXend,
	fMicroSecs const   usIncrement,
	fMicroSecs const   usInWindow
) const
{
	fPIXEL     const   fPixYoffset { fPixTrackHeight * Cast2Float(idSignal.GetTrackNr().GetValue()+1) };
	Signal     const & signal      { m_pMonitorData->GetSignal( idSignal ) }; 
	fMicroSecs const   usEnd       { m_pMRI->GetSimulationTime( ) };

	fPIXEL const fPixYvalue { getYvalue( signal, usEnd ) };
	if ( isnan(fPixYvalue.GetValue()) )
		return;

	fPIXEL            fPixWidth { 1.0_fPIXEL };
	fPIXEL            fPixYoff  { fPixYoffset };
	fPixelPoint       prevPoint { fPixXend, fPixYvalue + fPixYoffset };
	fMicroSecs  const timeStart { max( usEnd - usInWindow, signal.GetStartTime() ) };

	if ( idSignal == m_idSigSelected )  
	{
		fPixWidth = 3.0_fPIXEL;                          // emphasize selected signal                
		fPixYoff += Convert2fPIXEL( m_pixMoveOffsetY );  // may have move offset
	}

	for ( fMicroSecs time = usEnd - usIncrement; time >= timeStart; time -= usIncrement )
	{
		float       const fTicks   { (usEnd - time) / m_fMicroSecsPerPixel };
		fPIXEL      const fPixX    { fPixXend - fPIXEL(fTicks) };
		fPixelPoint const actPoint { fPixX, fPixYoff - getYvalue( signal, time ) };
		m_graphics.DrawLine( prevPoint, actPoint, fPixWidth, NNetColors::COL_BLACK );
		prevPoint = actPoint;
	}
}

void MonitorWindow::doPaint( ) const
{
	if ( m_pMonitorData->NoTracks() )
		return;

	PixelPoint const pixPointCrsr { GetRelativeCrsrPosition( ) };
	fPIXEL     const fPWidth      { Convert2fPIXEL( GetClientWindowWidth( ) ) };
	fMicroSecs const usInWin      { m_fMicroSecsPerPixel * fPWidth.GetValue() };
	fMicroSecs const usResolution { m_pParams->GetTimeResolution( ) };
	float      const fPointsInWin { usInWin / usResolution };
	fMicroSecs const usIncr       { (fPointsInWin > fPWidth.GetValue()) ? m_fMicroSecsPerPixel : usResolution };
	fPIXEL     const fPHeight     { calcTrackHeight() };

	m_pMonitorData->Apply2AllSignals( [&]( SignalId const id ) { paintSignal( id, fPHeight, fPWidth, usIncr, usInWin ); } );

	if ( m_bShowScale )
		m_scale.DisplayStaticScale( );

	if ( m_trackNrHighlighted.IsNotNull() )  // paint background of selected track
	{
		fPixelPoint    const pos  {	0._fPIXEL, fPHeight * Cast2Float(m_trackNrHighlighted.GetValue()) };
		fPixelRectSize const size {	fPWidth, fPHeight };
		m_graphics.DrawTranspRect( fPixelRect( pos, size ), NNetColors::COL_BEACON );
	}

	m_measurement.DisplayDynamicScale( Convert2fPIXEL(pixPointCrsr.GetX()), m_fMicroSecsPerPixel );
}

bool MonitorWindow::testSignal  // if signal is "better" than fPixBestDelta, update fPixBestDelta and return true, else false
( 
	Signal     const & signal,
	fMicroSecs const   umTime,
	fPIXEL     const   fPixCrsrOffset,  // vertical distance from crsr pos to zero line of track
	fPIXEL           & fPixBestDelta
) const
{
	if ( umTime >= signal.GetStartTime() )
	{
		fPIXEL const fPixAmplitude { getYvalue( signal, umTime ) };
		fPIXEL const fPixDelta     { fPixAmplitude - fPixCrsrOffset };
		fPIXEL const fPixDeltaAbs  { fPixDelta.GetAbs( ) };
		if ( fPixDeltaAbs < fPixBestDelta )
		{
			fPixBestDelta = fPixDeltaAbs;
			return true;
		}
	}
	return false;
}

SignalNr const MonitorWindow::findSignal( TrackNr const trackNr, PixelPoint const & ptCrsr ) const
{
	SignalNr signalNrRes { SignalNr::NULL_VAL() };

	if ( m_pMonitorData->NoTracks() )
		return signalNrRes;
	if ( trackNr.IsNull() )
		return signalNrRes;

	fPIXEL      const fPixWidth       { Convert2fPIXEL( GetClientWindowWidth( ) ) };
	fMicroSecs  const usInWindow      { m_fMicroSecsPerPixel * fPixWidth.GetValue() };
	fMicroSecs  const usEnd           { m_pMRI->GetSimulationTime( ) };
	fMicroSecs  const timeStart       { usEnd - usInWindow };
	fPixelPoint const fPixPtCrsr      { Convert2fPixelPoint( ptCrsr ) };
	fMicroSecs  const umTime          { timeStart + m_fMicroSecsPerPixel * fPixPtCrsr.GetXvalue() };
	fPIXEL      const fPixTrackHeight { calcTrackHeight() };
	fPIXEL            fPixYoffset     { fPixTrackHeight };
	fPIXEL            fPixBestDelta   { fPIXEL::MAX_VAL() };
	m_pMonitorData->Apply2AllSignalsInTrack
	(
		trackNr,
		[&](SignalNr const signalNr)
		{
			if ( 
				  testSignal
				  ( 						
					  m_pMonitorData->GetSignal( SignalId(trackNr, signalNr) ),
					  umTime, 
					  fPixYoffset - fPixPtCrsr.GetY(), 
					  fPixBestDelta 
				  ) 
			   )
			{
				signalNrRes = signalNr;
			}
		}
	);
	fPixYoffset += fPixTrackHeight;
	return signalNrRes;
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
	return true;
}

bool MonitorWindow::OnShow( WPARAM const wParam, LPARAM const lParam )
{
	bool bShow { static_cast<bool>(wParam) };
	if ( bShow )
	{
		m_measurement.ResetLimits( );
		if ( m_idSigSelected != SignalIdNull )
			m_pBeaconAnimation->Start( m_pMonitorData->GetSignal( m_idSigSelected ).GetSignalSource() );
	}
	else 
		m_pBeaconAnimation->Stop( );
	return false;
}

void MonitorWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam( lParam ) };
	m_trackNrHighlighted = findTrack( pixCrsrPos.GetY() );

	if ( wParam & MK_LBUTTON )
	{
		if ( m_pixLast.IsNotNull( ) )
		{
			if ( m_measurement.TrackingActive() )
				m_measurement.MoveSelection( Convert2fPIXEL( pixCrsrPos.GetX() ) );
			else if ( m_idSigSelected.GetSignalNr().IsNotNull() )
				m_pixMoveOffsetY += pixCrsrPos.GetY() - m_pixLast.GetY();
		}
		m_pixLast = pixCrsrPos;
		Trigger( );   // cause repaint
	}
	else  // left button not pressed: select signal
	{
		if ( m_measurement.Select( Convert2fPIXEL( pixCrsrPos.GetX() ) ) )
			Trigger( );

		selectSignal( SignalId( m_trackNrHighlighted, findSignal( m_trackNrHighlighted, pixCrsrPos ) ) );
	}
	(void)TrackMouseEvent( & m_trackStruct );
}

void MonitorWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam ) 
{
	if ( m_idSigSelected.GetTrackNr() != m_trackNrHighlighted )
	{
		m_idSigSelected = m_pMonitorData->MoveSignal( m_idSigSelected, m_trackNrHighlighted );
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLast.Set2Null();
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

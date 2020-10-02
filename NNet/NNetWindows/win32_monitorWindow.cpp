// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include <assert.h>
#include "util.h"
#include "scale.h"
#include "Signal.h"
#include "Resource.h"
#include "BaseKnot.h"
#include "BeaconAnimation.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "win32_monitorWindow.h"

using std::find;


void MonitorWindow::Start
( 
	HWND                     const   hwndParent,
	NNetModelReaderInterface const & model,
	Param                    const & params,
	BeaconAnimation                & beaconAnimation
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
	m_pBeaconAnimation = & beaconAnimation;
	m_pParams          = & params;
	m_pModel           = & model;
	m_graphics.Initialize( hwnd );
	SetWindowText( hwnd, L"Monitor" );
	m_trackStruct.hwndTrack = hwnd;
}

void MonitorWindow::Stop( )
{
	m_graphics.ShutDown( );
	DestroyWindow( );
}

long MonitorWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_bRuler )
		AppendMenu( hPopupMenu, MF_STRING, IDD_RULER_OFF, L"Ruler off" );
	else 
		AppendMenu( hPopupMenu, MF_STRING, IDD_RULER_ON,  L"Ruler on" );

	AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_TRACK, L"Delete track" );
	AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_TRACK,    L"Add track" );

	if ( m_pSelectedSignal )
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL, L"Delete signal" );

	return 0L; // will be forwarded to HandleContextMenuCommand
}

void MonitorWindow::selectSignal( Signal * const pSignal )
{
	if ( pSignal != m_pSelectedSignal )
	{
		if ( m_pSelectedSignal )
			m_pBeaconAnimation->Stop( );
		if ( pSignal )
			m_pBeaconAnimation->Start( pSignal->GetSignalSource() );
		else
		{
			m_selectedTrackNr   .Set2Null();
			m_trackNrOfSelSignal.Set2Null();
			m_pixLastY         .Set2Null();
			m_pixMoveOffsetY = 0_PIXEL;
		}
		m_pSelectedSignal = pSignal;
		Trigger( );   // cause repaint
	}
}

void MonitorWindow::moveSignal( PIXEL const pixY )
{
	if ( m_pixLastY.IsNotNull( ) && m_pSelectedSignal )
		m_pixMoveOffsetY += pixY - m_pixLastY;
	m_pixLastY = pixY;
	Trigger( );   // cause repaint
}

TrackNr const MonitorWindow::findPos4NewTrack( PIXEL const pixCrsrPosY )
{
	fPIXEL const fPixCrsrYpos     { Convert2fPIXEL( pixCrsrPosY ) };
	int    const iTrackNr         { CastToInt( fPixCrsrYpos / m_fPixTrackHeight ) };
	fPIXEL const fPixTrackCenterY { m_fPixTrackHeight * (CastToFloat(iTrackNr) + 0.5f) };
	TrackNr      trackNr          { m_selectedTrackNr };
	if ( fPixCrsrYpos > fPixTrackCenterY )
		++trackNr;
	return trackNr;
}

void MonitorWindow::addTrack( TrackNr const pos )
{
	m_monitor.InsertTrack( pos );
	updateTrackHeight( );
	Show( true );            // if window was not visible, show it now
}

void MonitorWindow::AddSignal( Signal & signal ) 
{ 
	TrackNr posOfNewTrack { m_monitor.GetNrOfTracks() };
	addTrack( posOfNewTrack );
	m_monitor.GetTrack( posOfNewTrack ).AddSignal( & signal );
	signal.RegisterObserver( this );
}

void MonitorWindow::updateTrackHeight( )
{
	PIXEL const pixRectHeight  { GetClientWindowHeight( ) };
	PIXEL const pixExtraSpace  { m_bRuler ? 60_PIXEL : 10_PIXEL };
	PIXEL const pixFreeHeight  { pixRectHeight - pixExtraSpace };
	PIXEL const pixTrackHeight { 
		                          m_monitor.NoTracks() 
		                          ? pixFreeHeight 
		                          : pixFreeHeight / CastToLong(m_monitor.GetNrOfTracks()) 
	                           }; 
	m_fPixTrackHeight = Convert2fPIXEL( pixTrackHeight );
}

fPIXEL const MonitorWindow::getYvalue( Signal const & signal, fMicroSecs const time )
{
	float  const fDataPoint { signal.GetDataPoint( time ) };
	fPIXEL const fPixYvalue { fDataPoint / m_fYvaluesPerPixel };
	return fPixYvalue;
}

void MonitorWindow::paintSignal
( 
	Signal     const & signal, 
	fPIXEL     const   fPixYoffset,
	fPIXEL     const   fPixXend,
	fMicroSecs const   usIncrement,
	fMicroSecs const   usEnd,
	fMicroSecs const   usInWindow
)
{
	fPIXEL const fPixYvalue { getYvalue( signal, usEnd ) };
	if ( isnan(fPixYvalue.GetValue()) )
		return;

	fPIXEL            fPixWidth { 1.0_fPIXEL };
	fPIXEL            fPixYoff  { fPixYoffset };
	fPixelPoint       prevPoint { fPixXend, fPixYvalue + fPixYoffset };
	fMicroSecs  const timeStart { max( usEnd - usInWindow, signal.GetStartTime() ) };

	if ( & signal == m_pSelectedSignal )  
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

void MonitorWindow::doPaint( )
{
	if ( m_monitor.NoTracks() )
		return;

	fPIXEL     const fPixWidth    { Convert2fPIXEL( GetClientWindowWidth( ) ) };
	fMicroSecs const usInWindow   { m_fMicroSecsPerPixel * fPixWidth.GetValue() };
	fMicroSecs const usEnd        { m_pModel->GetSimulationTime( ) };
	fMicroSecs const usResolution { m_pParams->GetTimeResolution( ) };
	float      const fPointsInWin { usInWindow / usResolution };
	fMicroSecs const usIncrement  { (fPointsInWin > fPixWidth.GetValue()) ? m_fMicroSecsPerPixel : usResolution };
	fPIXEL           fPixYoffset  { m_fPixTrackHeight };
	m_monitor.Apply2AllTracks
	( 
		[&]( Track const & track )
		{
			track.Apply2AllSignals(	[&](Signal const & signal) { paintSignal( signal, fPixYoffset, fPixWidth, usIncrement, usEnd, usInWindow ); } );
			fPixYoffset += m_fPixTrackHeight;
		}
	);

	if ( m_bRuler )
		Scale::Display( m_graphics, GetClRectSize(), m_fMicroSecsPerPixel.GetValue(), L"s" );

	if ( m_selectedTrackNr.IsNotNull() )  // paint background of selected track
	{
		fPixelPoint    const pos  {	0._fPIXEL, m_fPixTrackHeight * CastToFloat(m_selectedTrackNr.GetValue()) };
		fPixelRectSize const size {	fPixWidth, m_fPixTrackHeight };
		m_graphics.DrawTranspRect( fPixelRect( pos, size ), NNetColors::COL_BEACON );
	}
}

Signal * MonitorWindow::findSignal( TrackNr const trackNr )
{
	m_trackNrOfSelSignal.Set2Null();
	if ( m_monitor.NoTracks() )
		return nullptr;
	if ( trackNr.IsNull() )
		return nullptr;

	fPIXEL      const fPixWidth     { Convert2fPIXEL( GetClientWindowWidth( ) ) };
	fMicroSecs  const usInWindow    { m_fMicroSecsPerPixel * fPixWidth.GetValue() };
	fMicroSecs  const usEnd         { m_pModel->GetSimulationTime( ) };
	fMicroSecs  const timeStart     { usEnd - usInWindow };
	PixelPoint  const ptCrsr        { GetRelativeCrsrPosition( ) };
	fPixelPoint const fPixPtCrsr    { Convert2fPixelPoint( ptCrsr ) };
	fMicroSecs  const umTime        { timeStart + m_fMicroSecsPerPixel * fPixPtCrsr.GetXvalue() };
	fPIXEL            fPixYoffset   { m_fPixTrackHeight };
	fPIXEL            fPixBestDelta { fPIXEL::MAX_VAL() };
	Signal          * pSignalFound  { nullptr };
	m_monitor.GetTrack( trackNr ).Apply2AllSignals
	(
		[&]( Signal & signal )
		{
			if ( testSignal( signal, umTime, fPixYoffset - fPixPtCrsr.GetY(), fPixBestDelta ) )
			{
				m_trackNrOfSelSignal = trackNr;
				pSignalFound         = & signal;
			}
		}
	);
	fPixYoffset += m_fPixTrackHeight;
	return pSignalFound;
}

TrackNr const MonitorWindow::findTrack( PIXEL const pixPosY )
{
	fPIXEL     const fPixCrsrYpos { Convert2fPIXEL( pixPosY ) };
	TrackNr    const trackNr      { CastToInt( fPixCrsrYpos / m_fPixTrackHeight ) };
	return m_monitor.IsValid( trackNr ) ? trackNr : TrackNr::NULL_VAL();
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
	case IDD_RULER_ON:
		m_bRuler = true;
		break;

	case IDD_RULER_OFF:
		m_bRuler = false;
		break;

	case IDD_DELETE_SIGNAL:		
		m_monitor.DeleteSignal( m_pSelectedSignal, m_selectedTrackNr );
		selectSignal( nullptr );
		break;

	case IDD_ADD_TRACK:
		addTrack( findPos4NewTrack( pixPoint.GetY() ) );
		break;

	case IDD_DELETE_TRACK:
		m_monitor.DeleteTrack( m_selectedTrackNr );
		m_selectedTrackNr.Set2Null();
		selectSignal( nullptr );
		updateTrackHeight( );
		if ( m_monitor.NoTracks() )
			SendMessage( WM_COMMAND, IDM_WINDOW_OFF, 0 );
		break;

	default:
		break;
	}

	return BaseWindow::OnCommand( wParam, lParam, pixPoint );
}

bool MonitorWindow::OnMouseLeave( WPARAM const wParam, LPARAM const lParam )
{
	if ( ! CrsrInClientRect() )
	{
		selectSignal( nullptr );
	}
	return false;
}

bool MonitorWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize( width, height );
	updateTrackHeight( );
	return true;
}

void MonitorWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( m_monitor.NoTracks() )
		return;

	m_selectedTrackNr = findTrack( CrsrYpos( lParam ) );

	if ( wParam & MK_LBUTTON )
		moveSignal( CrsrYpos( lParam ) );
	else
		selectSignal( findSignal( m_selectedTrackNr ) );

	(void)TrackMouseEvent( & m_trackStruct );
}

void MonitorWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam ) 
{
	if ( m_selectedTrackNr.IsNotNull() && (m_selectedTrackNr != m_trackNrOfSelSignal) )
	{
		m_monitor.RemoveSignalFromTrack( m_pSelectedSignal, m_trackNrOfSelSignal );
		m_monitor.GetTrack( m_selectedTrackNr ).AddSignal( m_pSelectedSignal );
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLastY = PIXEL::NULL_VAL();
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

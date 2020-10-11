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
	m_pModel           = & model;
	m_pMonitorData     = & monitorData;
	m_graphics.Initialize( hwnd );
	SetWindowText( hwnd, L"Monitor" );
	m_trackStruct.hwndTrack = hwnd;
	m_trackStruct.dwFlags   = TME_LEAVE;
}

void MonitorWindow::Reset( )
{
	selectSignal( SignalNr::NULL_VAL() );
	m_trackStruct.hwndTrack = HWND(0);
	m_trackStruct.dwFlags   = TME_CANCEL;
	(void)TrackMouseEvent( & m_trackStruct );
}

void MonitorWindow::Stop( )
{
	Reset( );
	m_pBeaconAnimation = nullptr;
	m_pParams          = nullptr;
	m_pModel           = nullptr;
	m_pMonitorData     = nullptr;
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

	if ( m_selectedSignalNr.IsNotNull() )
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SIGNAL, L"Delete signal" );

	return 0L; // will be forwarded to HandleContextMenuCommand
}

void MonitorWindow::selectSignal( SignalNr const signalNr )
{
	if ( signalNr != m_selectedSignalNr )
	{
		if ( m_selectedSignalNr.IsNotNull() )
			m_pBeaconAnimation->Stop( );

		if ( Signal const * const pSignal { m_pMonitorData->GetSignal( m_selectedTrackNr, signalNr ) } )
		{
			m_pBeaconAnimation->Start( pSignal->GetSignalSource() );
		}
		else
		{
			m_selectedTrackNr.Set2Null();
			m_pixLastY       .Set2Null();
		}

		m_pixMoveOffsetY = 0_PIXEL;
		m_selectedSignalNr = signalNr;
		Trigger( );   // cause repaint
	}
}

void MonitorWindow::moveSignal( PIXEL const pixY )
{
	if ( m_pixLastY.IsNotNull( ) && m_selectedSignalNr.IsNotNull() )
		m_pixMoveOffsetY += pixY - m_pixLastY;
	m_pixLastY = pixY;
	Trigger( );   // cause repaint
}

TrackNr const MonitorWindow::findPos4NewTrack( PIXEL const pixCrsrPosY ) const
{
	fPIXEL const fPixTrackHeight  { calcTrackHeight() };
	fPIXEL const fPixCrsrYpos     { Convert2fPIXEL( pixCrsrPosY ) };
	int    const iTrackNr         { Cast2Int( fPixCrsrYpos / fPixTrackHeight ) };
	fPIXEL const fPixTrackCenterY { fPixTrackHeight * (Cast2Float(iTrackNr) + 0.5f) };
	TrackNr      trackNr          { m_selectedTrackNr };
	if ( fPixCrsrYpos > fPixTrackCenterY )
		++trackNr;
	return trackNr;
}

void MonitorWindow::addTrack( TrackNr const trackNr )
{
	m_pController->HandleCommand( IDD_INSERT_TRACK, trackNr.GetValue() );
	Show( true );            // if window was not visible, show it now
}

fPIXEL const MonitorWindow::calcTrackHeight( ) const
{
	PIXEL const pixRectHeight  { GetClientWindowHeight( ) };
	PIXEL const pixExtraSpace  { m_bRuler ? 60_PIXEL : 10_PIXEL };
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
	Signal     const & signal, 
	fPIXEL     const   fPixYoffset,
	fPIXEL     const   fPixXend,
	fMicroSecs const   usIncrement,
	fMicroSecs const   usInWindow,
	bool       const   bSelected
) const
{
	fMicroSecs const usEnd { m_pModel->GetSimulationTime( ) };

	fPIXEL const fPixYvalue { getYvalue( signal, usEnd ) };
	if ( isnan(fPixYvalue.GetValue()) )
		return;

	fPIXEL            fPixWidth { 1.0_fPIXEL };
	fPIXEL            fPixYoff  { fPixYoffset };
	fPixelPoint       prevPoint { fPixXend, fPixYvalue + fPixYoffset };
	fMicroSecs  const timeStart { max( usEnd - usInWindow, signal.GetStartTime() ) };

	if ( bSelected )  
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

	fPIXEL     const fPixWidth       { Convert2fPIXEL( GetClientWindowWidth( ) ) };
	fMicroSecs const usInWindow      { m_fMicroSecsPerPixel * fPixWidth.GetValue() };
	fMicroSecs const usResolution    { m_pParams->GetTimeResolution( ) };
	float      const fPointsInWin    { usInWindow / usResolution };
	fMicroSecs const usIncrement     { (fPointsInWin > fPixWidth.GetValue()) ? m_fMicroSecsPerPixel : usResolution };
	fPIXEL     const fPixTrackHeight { calcTrackHeight() };
	fPIXEL           fPixYoffset     { fPixTrackHeight };
	m_pMonitorData->CheckTracks( );
	m_pMonitorData->Apply2AllTracks
	( 
		[&]( TrackConstIter const itTrack )
		{
			bool bSelectedTrack { m_pMonitorData->GetTrackNr( itTrack ) == m_selectedTrackNr };
			itTrack->Apply2AllSignalsC
			( 
				[&](SignalIter const itSignal) 
				{ 
					assert( *itSignal != nullptr );
					bool bSelectedSignal { m_pMonitorData->GetSignalNr( itTrack, itSignal ) == m_selectedSignalNr };
					paintSignal( **itSignal, fPixYoffset, fPixWidth, usIncrement, usInWindow, bSelectedTrack && bSelectedSignal ); 
				} 
			);
			fPixYoffset += fPixTrackHeight;
		}
	);

	if ( m_bRuler )
		Scale::Display( m_graphics, GetClRectSize(), m_fMicroSecsPerPixel.GetValue(), L"s" );

	if ( m_selectedTrackNr.IsNotNull() )  // paint background of selected track
	{
		fPixelPoint    const pos  {	0._fPIXEL, fPixTrackHeight * Cast2Float(m_selectedTrackNr.GetValue()) };
		fPixelRectSize const size {	fPixWidth, fPixTrackHeight };
		m_graphics.DrawTranspRect( fPixelRect( pos, size ), NNetColors::COL_BEACON );
	}
}

bool MonitorWindow::testSignal  // if signal is "better" than fPixBestDelta, update fPixBestDelta and return true, else false
( 
	Signal     const & signal,
	fMicroSecs const   umTime,
	fPIXEL     const   fPixOffset,
	fPIXEL           & fPixBestDelta
) const
{
	if ( umTime >= signal.GetStartTime() )
	{
		fPIXEL const fPixYvalueAbs { getYvalue( signal, umTime ) };
		fPIXEL const fPixDelta     { fPixYvalueAbs + fPixOffset };
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
	fMicroSecs  const usEnd           { m_pModel->GetSimulationTime( ) };
	fMicroSecs  const timeStart       { usEnd - usInWindow };
	fPixelPoint const fPixPtCrsr      { Convert2fPixelPoint( ptCrsr ) };
	fMicroSecs  const umTime          { timeStart + m_fMicroSecsPerPixel * fPixPtCrsr.GetXvalue() };
	fPIXEL      const fPixTrackHeight { calcTrackHeight() };
	fPIXEL            fPixYoffset     { fPixTrackHeight };
	fPIXEL            fPixBestDelta   { fPIXEL::MAX_VAL() };
	m_pMonitorData->Apply2AllSignals
	(
		trackNr,
		[&](SignalIter const itSignal)
		{
			if ( testSignal( **itSignal, umTime, fPixYoffset - fPixPtCrsr.GetY(), fPixBestDelta ) )
			{
				signalNrRes = m_pMonitorData->GetSignalNr( trackNr, itSignal );
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
	case IDD_RULER_ON:
		m_bRuler = true;
		break;

	case IDD_RULER_OFF:
		m_bRuler = false;
		break;

	case IDD_DELETE_SIGNAL:
		{
			Signal * const pSignal { m_pMonitorData->RemoveSignal( m_selectedTrackNr, m_selectedSignalNr ) };
			delete pSignal;
			selectSignal( SignalNr::NULL_VAL() );
		}
		break;

	case IDD_ADD_TRACK:
		addTrack( findPos4NewTrack( pixPoint.GetY() ) );
		break;

	case IDD_DELETE_TRACK:
		m_pMonitorData->DeleteTrack( m_selectedTrackNr );
		m_selectedTrackNr.Set2Null();
		selectSignal( SignalNr::NULL_VAL() );
		if ( m_pMonitorData->NoTracks() )
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
		selectSignal( SignalNr::NULL_VAL() );
	}
	return false;
}

bool MonitorWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize( width, height );
	return true;
}

void MonitorWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam ) 
{
	m_originalNr = m_selectedTrackNr;
	(void)TrackMouseEvent( & m_trackStruct );
};

void MonitorWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const pixCrsrPos { GetCrsrPosFromLparam( lParam ) };

	if ( wParam & MK_LBUTTON )
	{
		moveSignal( pixCrsrPos.GetY() );
	}
	else  // left button not pressed: select signal
	{
		m_selectedTrackNr = findTrack( pixCrsrPos.GetY() );
		SignalNr signalNr { findSignal( m_selectedTrackNr, pixCrsrPos ) };
		selectSignal( signalNr );
	}
}

void MonitorWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam ) 
{
	if ( m_selectedTrackNr.IsNotNull() && (m_selectedTrackNr != m_originalNr) )
	{
		m_selectedSignalNr = m_pMonitorData->MoveSignal( m_originalNr, m_selectedSignalNr, m_selectedTrackNr );
		m_originalNr.Set2Null();
	}
	m_pixMoveOffsetY = 0_PIXEL;
	m_pixLastY.Set2Null();
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

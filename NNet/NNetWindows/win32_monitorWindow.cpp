// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include "util.h"
#include "scale.h"
#include "Signal.h"
#include "Resource.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "win32_monitorWindow.h"

void MonitorWindow::Start
( 
	HWND                     const   hwndParent,
	NNetModelReaderInterface const & model,
	Param                    const & params 
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
	m_pParams = & params;
	m_pModel  = & model;
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

	if ( m_iSelectedSignal >= 0 )
		AppendMenu( hPopupMenu, MF_STRING, IDD_REMOVE_SIGNAL, L"Remove signal" );

	return 0L; // will be forwarded to HandleContextMenuCommand
}

PIXEL const MonitorWindow::getHeight4Signal( PIXEL pixCLientHeight )
{
	PIXEL const HEIGHT4SCALE  { 60_PIXEL };
	PIXEL const pixFreeHeight { pixCLientHeight - (m_bRuler ? HEIGHT4SCALE : 10_PIXEL ) };
	return pixFreeHeight /  CastToLong(m_Signals.size()); 
}

fPIXEL const MonitorWindow::getYvalue( Signal const & signal, fMicroSecs const time )
{
	float  const fDataPoint { signal.GetDataPoint( time ) };
	fPIXEL const fPixYvalue { fDataPoint / m_fYvaluesPerPixel };
	return fPixYvalue;
}

void MonitorWindow::AddSignal( Signal & signal ) 
{ 
	m_Signals.push_back( & signal );
	signal.RegisterObserver( this );
}

void MonitorWindow::paintSignal
( 
	Signal     const & signal, 
	fPIXEL     const   fPixYoffset,
	fPIXEL     const   fPixXend,
	fMicroSecs const   usIncrement,
	fMicroSecs const   timeEnd,
	fMicroSecs const   usInWindow
)
{
	fMicroSecs const timeStart   { max( timeEnd - usInWindow, signal.GetStartTime() ) };
	fPIXEL     const fPixYvalue  { getYvalue( signal, timeEnd ) };
	if ( ! isnan(fPixYvalue.GetValue()) )
	{
		fPixelPoint prevPoint { fPixXend, fPixYvalue + fPixYoffset };
		for ( fMicroSecs time = timeEnd - usIncrement; time >= timeStart; time -= usIncrement )
		{
			float       const fTicks   { (timeEnd - time) / m_fMicroSecsPerPixel };
			fPIXEL      const fPixX    { fPixXend - fPIXEL(fTicks) };
			fPixelPoint const actPoint { fPixX, fPixYoffset - getYvalue( signal, time ) };
			m_graphics.DrawLine( prevPoint, actPoint, 1.0_fPIXEL, NNetColors::COL_BLACK );
			prevPoint = actPoint;
		}
	}
}

void MonitorWindow::doPaint( )
{
	PixelRectSize  rectClient { GetClRectSize( ) };
	fPixelRectSize rectSize   { Convert2fPixelRectSize( rectClient ) };

	float      const fSizeX      { rectSize.GetX().GetValue() };
	fMicroSecs const usResolution{ m_pParams->GetTimeResolution( ) };
	fMicroSecs const usInWindow  { m_fMicroSecsPerPixel * fSizeX };
	float      const fPointsInWin{ usInWindow / usResolution };
	fMicroSecs const timeEnd     { m_pModel->GetSimulationTime( ) };
	fMicroSecs const usIncrement { (fPointsInWin > fSizeX) ? m_fMicroSecsPerPixel : usResolution };

	if ( m_Signals.empty() )
		return;

	PIXEL const pixHeight4Signal { getHeight4Signal( rectClient.GetY() ) };
	PIXEL       pixYoffset       { pixHeight4Signal };
	for ( Signal const * pSignal : m_Signals )
	{
		paintSignal
		( 
			* pSignal, 
			Convert2fPIXEL( pixYoffset ),
			rectSize.GetX(), 
			usIncrement, 
			timeEnd, 
			usInWindow 
		);
		pixYoffset += pixHeight4Signal;
	}

	if ( m_bRuler )
	{
		Scale::Display
		( 
			m_graphics, 
			GetClRectSize(), 
			m_fMicroSecsPerPixel.GetValue(), 
			L"s" 
		);
	}

	if ( m_iSelectedSignal >= 0 )
	{
		fPixelRect const fPixRect
		{
			0._fPIXEL,                                                   // left
			Convert2fPIXEL( pixHeight4Signal * m_iSelectedSignal ),      // top
			rectSize.GetX(),                                             // right
			Convert2fPIXEL( pixHeight4Signal * (m_iSelectedSignal + 1) ) // bottom
		};
		m_graphics.DrawTranspRect( fPixRect, NNetColors::SELECTION_RECT );
	}
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

	case IDD_REMOVE_SIGNAL:
		{
			Signal const * pSignal { m_Signals[m_iSelectedSignal] };
			auto res = find( begin(m_Signals), end(m_Signals), pSignal );
			m_Signals.erase( res );
			delete pSignal;
		}
		break;

	default:
		return false;
	}

	return BaseWindow::OnCommand( wParam, lParam, pixPoint );
}

bool MonitorWindow::OnMouseLeave( WPARAM const wParam, LPARAM const lParam )
{
	m_iSelectedSignal = -1;
	return false;
}

bool MonitorWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize( width, height );
	return true;
}

void MonitorWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( ! m_Signals.empty() )
	{
		PixelPoint const ptCrsr           { GetRelativeCrsrPosition( ) };
		PIXEL      const pixHeight4Signal { getHeight4Signal( GetClientWindowHeight( ) ) };
		m_iSelectedSignal = ptCrsr.GetY() / pixHeight4Signal;
		(void)TrackMouseEvent( & m_trackStruct );
	}
}

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

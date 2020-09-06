// win32_monitorWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <math.h>    
#include "Signal.h"
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
}

void MonitorWindow::Stop( )
{
	m_graphics.ShutDown( );
	DestroyWindow( );
}

void MonitorWindow::SetSignal( Signal const & signal ) 
{ 
	m_pSignal = & signal; 
}

void MonitorWindow::doPaint( )
{
	fPixelRectSize rectSize { Convert2fPixelRectSize( GetClRectSize( ) ) };

	float      const fSizeX          { rectSize.GetX().GetValue() };
	fPIXEL     const fPixYoffset     { rectSize.GetY() / 2.0f };
	fMicroSecs const usResolution    { m_pParams->GetTimeResolution( ) };
	fMicroSecs const usInWindow      { m_fMicroSecsPerPixel * fSizeX };
	float      const fPointsInWindow { usInWindow / usResolution };
	fMicroSecs const timeEnd         { m_pModel->GetSimulationTime( ) };
	fMicroSecs const timeStart       { max( timeEnd - usInWindow, m_pSignal->GetStartTime() ) };
	fMicroSecs const usIncrement     { (fPointsInWindow > fSizeX) ? m_fMicroSecsPerPixel : usResolution };
	fPIXEL     const fPixYvalue      { getYvalue( timeEnd ) };
	if ( ! isnan(fPixYvalue.GetValue()) )
	{
		fPixelPoint previousPoint { rectSize.GetX(), fPixYvalue + fPixYoffset };
		for ( fMicroSecs time = timeEnd - usIncrement; time >= timeStart; time -= usIncrement )
		{
			float       const fTicks      { (timeEnd - time) / m_fMicroSecsPerPixel };
			fPIXEL      const fPixX       { rectSize.GetX() - fPIXEL(roundf(fTicks)) };
			fPixelPoint const actualPoint { fPixX, fPixYoffset - getYvalue( time ) };
			m_graphics.DrawLine( previousPoint, actualPoint, 1.0_fPIXEL, NNetColors::COL_BLACK );
			previousPoint = actualPoint;
		}
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

bool MonitorWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_graphics.Resize( width, height );
	return true;
}

void MonitorWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{  
	static float const ZOOM_FACTOR { 1.3f };

	int  const iDelta     { GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA };
	bool const bDirection { iDelta > 0 };

	for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
	{
		float const fFactor { bDirection ? 1.0f / ZOOM_FACTOR : ZOOM_FACTOR };
		m_fMicroSecsPerPixel *= fFactor;
	}
}

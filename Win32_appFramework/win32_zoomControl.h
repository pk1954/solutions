// win32_zoomControl.h
//
// win32_appFramework

#pragma once

#include "LogarithmicTrackbar.h"
#include "win32_util_resource.h"
#include "win32_tooltip.h"
#include "win32_status.h"

class ZoomControl
{
public:

	static void AddSizeControl
	( 
		StatusBar * pStatusBar, 
		double const dMin, 
		double const dMax, 
		double const dDefault
	)
	{ 
		HWND hwndStatusBar = pStatusBar->GetWindowHandle( );

		pStatusBar->AddStaticControl( L"Size" );
		pStatusBar->AddButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT, BS_PUSHBUTTON ); 
		pStatusBar->AddTrackBar     (             (HMENU)IDM_ZOOM_TRACKBAR ); 
		pStatusBar->AddButton       ( L" + ",     (HMENU)IDM_ZOOM_IN,  BS_PUSHBUTTON ); 
		pStatusBar->AddButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM, BS_PUSHBUTTON ); 

		long const lMinPos = CastToLong( LogarithmicTrackbar::Value2TrackbarD( dMin ) );
		long const lMaxPos = CastToLong( LogarithmicTrackbar::Value2TrackbarD( dMax ) );

		pStatusBar->SetTrackBarRange( IDM_ZOOM_TRACKBAR, lMinPos, lMaxPos );  

		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_OUT,      L"Zoom out one step" );
		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_TRACKBAR, L"Move slider to zoom in or out" );
		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_IN,       L"Zoom in one step" );
		CreateBalloonToolTip( hwndStatusBar, IDM_FIT_ZOOM,      L"Zoom to maximum possible size, which fits in actual window" );

		pStatusBar->SetTrackBarPos( IDM_ZOOM_TRACKBAR, CastToLong( LogarithmicTrackbar::Value2TrackbarD( dDefault ) ) ); 
	} 

private:

};
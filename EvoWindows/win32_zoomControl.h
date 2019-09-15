// win32_zoomControl.h
//
// win32_utilities

#pragma once

#include "PixelTypes.h"
#include "LogarithmicTrackbar.h"
#include "win32_util_resource.h"
#include "win32_status.h"

class ZoomControl
{
public:

	static void SetSizeTrackBar( StatusBar * pStatusBar, PIXEL const pixFieldSize )
	{ 
		pStatusBar->SetTrackBarPos( IDM_ZOOM_TRACKBAR, LogarithmicTrackbar::Value2Trackbar( pixFieldSize.GetValue() ) ); 
	}

	static void AddSizeControl( StatusBar * pStatusBar, long const lMin, long const lMax )
	{ 
		HWND hwndStatusBar = pStatusBar->GetWindowHandle( );

		pStatusBar->AddStaticControl( L"Size" );
		pStatusBar->AddButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT, BS_PUSHBUTTON ); 
		pStatusBar->AddTrackBar     (             (HMENU)IDM_ZOOM_TRACKBAR ); 
		pStatusBar->AddButton       ( L" + ",     (HMENU)IDM_ZOOM_IN,  BS_PUSHBUTTON ); 
		pStatusBar->AddButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM, BS_PUSHBUTTON ); 

		LONG const lMinPos = LogarithmicTrackbar::Value2Trackbar( lMin );
		LONG const lMaxPos = LogarithmicTrackbar::Value2Trackbar( lMax );

		pStatusBar->SetTrackBarRange( IDM_ZOOM_TRACKBAR, lMinPos, lMaxPos );  

		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_OUT,      L"Zoom out one step" );
		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_TRACKBAR, L"Move slider to zoom in or out" );
		CreateBalloonToolTip( hwndStatusBar, IDM_ZOOM_IN,       L"Zoom in one step" );
		CreateBalloonToolTip( hwndStatusBar, IDM_FIT_ZOOM,      L"Zoom to maximum possible size, which fits in actual window" );
	} 

private:

};
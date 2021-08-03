// win32_zoomControl.h
//
// win32_utilities

#pragma once

#include "win32_util_resource.h"
#include "win32_tooltip.h"
#include "win32_status.h"

class ZoomControl
{
public:

	static void AddSizeControl
	(
		StatusBar * const pStatusBar, 
		double      const dMin, 
		double      const dMax, 
		double      const dDefault
	)
	{ 
		HWND hwndStatusBar = pStatusBar->GetWindowHandle();

		pStatusBar->AddStaticControl(L"Size");
		pStatusBar->AddButton       (L" - ",     (HMENU)IDM_ZOOM_OUT, BS_PUSHBUTTON); 
		pStatusBar->AddTrackBar     (            (HMENU)IDM_ZOOM_TRACKBAR); 
		pStatusBar->AddButton       (L" + ",     (HMENU)IDM_ZOOM_IN,  BS_PUSHBUTTON); 
		pStatusBar->AddButton       (L"  Fit  ", (HMENU)IDM_FIT_ZOOM, BS_PUSHBUTTON); 

		long const lMinPos = Cast2Long(dMin);
		long const lMaxPos = Cast2Long(dMax);

		pStatusBar->SetTrackBarRange(IDM_ZOOM_TRACKBAR, lMinPos, lMaxPos);  
		pStatusBar->SetTrackBarPos  (IDM_ZOOM_TRACKBAR, Cast2Long(dDefault)); 

		CreateBalloonToolTip(hwndStatusBar, IDM_ZOOM_OUT,      L"Zoom out one step");
		CreateBalloonToolTip(hwndStatusBar, IDM_ZOOM_TRACKBAR, L"Move slider to zoom in or out");
		CreateBalloonToolTip(hwndStatusBar, IDM_ZOOM_IN,       L"Zoom in one step");
		CreateBalloonToolTip(hwndStatusBar, IDM_FIT_ZOOM,      L"Zoom to maximum possible size, which fits in actual window");
	} 

private:

};
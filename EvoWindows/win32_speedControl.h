// win32_speedControl.h
//
// EvoWindows

#pragma once

#include "Resource.h"
#include "LogarithmicTrackbar.h"
#include "win32_status.h"

class SpeedControl
{
public:

	static long GetTrackBarPos( StatusBar * pStatusBar )
	{
		LONG const lLogicalPos = pStatusBar->GetTrackBarPos( IDM_SIMULATION_SPEED );
		LONG const lValue      = SPEED_TRACKBAR_MAX - lLogicalPos;
		LONG const lPos        = LogarithmicTrackbar::TrackBar2Value( lValue );
		EnableWindow( pStatusBar->GetDlgItem( IDM_MAX_SPEED ), TRUE );
		return lPos;
	}

	static void SetSpeedTrackBar( StatusBar * pStatusBar, long const dwDelay )
	{ 
		LONG const lPos = ( dwDelay == 0 )
			? 0
			: LogarithmicTrackbar::Value2Trackbar( dwDelay );
		pStatusBar->SetTrackBarPos( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MAX - lPos );                
	}

	static void Set2MaxSpeed( StatusBar * pStatusBar )
	{
		HWND hwndStatusBar = pStatusBar->GetWindowHandle( );
		SetSpeedTrackBar( pStatusBar, 0 );
		EnableWindow( GetDlgItem( hwndStatusBar, IDM_MAX_SPEED ), FALSE );
	}

	static void AddSimulationControl
	( 
		StatusBar * pStatusBar, 
		BOOL const bWithHistory 
	)
	{ 
		if ( bWithHistory )
			pStatusBar->AddButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS, BS_PUSHBUTTON );

		pStatusBar->AddButton  ( L"SingleStep", (HMENU)IDM_FORWARD,   BS_PUSHBUTTON ); 
		pStatusBar->AddButton  ( L"   Run    ", (HMENU)IDM_RUN,       BS_PUSHBUTTON ); 
		pStatusBar->AddButton  ( L"  Stop    ", (HMENU)IDM_STOP,      BS_PUSHBUTTON ); 
		pStatusBar->AddTrackBar(                (HMENU)IDM_SIMULATION_SPEED ); 
		pStatusBar->AddButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED, BS_PUSHBUTTON ); 

		pStatusBar->SetTrackBarRange( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
	} 
private:
	static long const MAX_DELAY;    // in msecs

	static long const SPEED_TRACKBAR_MIN; 
	static long const SPEED_TRACKBAR_MAX; 
};

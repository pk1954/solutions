// win32_speedControl.h
//
// EvoWindows

#pragma once

#include "Resource.h"
#include "LogarithmicTrackbar.h"
#include "win32_WorkThreadInterface.h"
#include "win32_status.h"

class SpeedControl
{
public:

	static long GetTrackBarPos( )
	{
		LONG const lLogicalPos = m_pStatusBar->GetTrackBarPos( IDM_SIMULATION_SPEED );
		LONG const lValue      = SPEED_TRACKBAR_MAX - lLogicalPos;
		LONG const lPos        = LogarithmicTrackbar::TrackBar2Value( lValue );
		EnableWindow( m_pStatusBar->GetDlgItem( IDM_MAX_SPEED ), TRUE );
		return lPos;
	}

	static void SetSpeedTrackBar( long const dwDelay )
	{ 
		LONG const lPos = ( dwDelay == 0 )
			? 0
			: LogarithmicTrackbar::Value2Trackbar( dwDelay );
		m_pStatusBar->SetTrackBarPos( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MAX - lPos );                
	}

	static void Set2MaxSpeed( )
	{
		HWND hwndStatusBar = m_pStatusBar->GetWindowHandle( );
		SetSpeedTrackBar( 0 );
		EnableWindow( GetDlgItem( hwndStatusBar, IDM_MAX_SPEED ), FALSE );
	}

	static void AddSimulationControl
	( 
		StatusBar     * const pStatusBar, 
		HistorySystem * const pHistorySystem,
		BOOL            const bWithHistory 
	)
	{ 
		m_bWithHistory   = bWithHistory;
		m_pStatusBar     = pStatusBar;
		m_pHistorySystem = pHistorySystem;

		if ( bWithHistory )
			pStatusBar->AddButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS, BS_PUSHBUTTON );

		m_pStatusBar->AddButton  ( L"SingleStep", (HMENU)IDM_FORWARD,   BS_PUSHBUTTON ); 
		m_pStatusBar->AddButton  ( L"   Run    ", (HMENU)IDM_RUN,       BS_PUSHBUTTON ); 
		m_pStatusBar->AddButton  ( L"  Stop    ", (HMENU)IDM_STOP,      BS_PUSHBUTTON ); 
		m_pStatusBar->AddTrackBar(                (HMENU)IDM_SIMULATION_SPEED ); 
		m_pStatusBar->AddButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED, BS_PUSHBUTTON ); 

		m_pStatusBar->SetTrackBarRange( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
	} 

	static void Adjust
	(
		BOOL                  const bIsRunning,
		WorkThreadInterface * const pWorkThreadInterface
    )
	{
		EnableWindow( m_pStatusBar->GetDlgItem( IDM_RUN  ), ! bIsRunning );
		EnableWindow( m_pStatusBar->GetDlgItem( IDM_STOP ),   bIsRunning );

		EnableWindow( m_pStatusBar->GetDlgItem( IDM_FORWARD ), ! bIsRunning );

		if ( m_bWithHistory )
		{
			BOOL bIsFirstGeneration = pWorkThreadInterface->GetCurrentGeneration( ) == 0;
			EnableWindow( m_pStatusBar-> GetDlgItem( IDM_BACKWARDS ), ! (bIsRunning || bIsFirstGeneration) );
		}
	}

private:
	static BOOL m_bWithHistory;

	static StatusBar     * m_pStatusBar;
	static HistorySystem * m_pHistorySystem;

	static long const MAX_DELAY;    // in msecs

	static long const SPEED_TRACKBAR_MIN; 
	static long const SPEED_TRACKBAR_MAX; 
};

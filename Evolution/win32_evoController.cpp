// win32_evoController.cpp
//
// Evolution

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "config.h"
#include "LogarithmicTrackbar.h"
#include "GridDimensions.h"
#include "EvoHistorySysGlue.h"
#include "win32_aboutBox.h"
#include "win32_evoAppWindow.h"
#include "win32_script.h"
#include "win32_stopwatch.h"
#include "win32_winManager.h"
#include "win32_delay.h"
#include "win32_speedControl.h"
#include "win32_zoomControl.h"
#include "win32_status.h"
#include "win32_EvoEditor.h"
#include "win32_evoAppMenu.h"
#include "win32_gridWindow.h"
#include "win32_packGridPoint.h"
#include "win32_colorManager.h"
#include "win32_resetDlg.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_evoController.h"

EvoController::EvoController
(
	WinManager        * const pWinManager,
	EvoHistorySysGlue * const pEvoHistGlue,
	Delay             * const pDelay,
	ColorManager      * const pColorManager,
	StatusBar         * const pStatusBar,
	GridWindow        * const pGridWindow,
	EvoEditorWindow   * const pEditorWindow
) :
	m_pAppWindow              ( nullptr ),
	m_pEvoWorkThreadInterface ( nullptr ),
	m_pWinManager             ( pWinManager   ),
	m_pEvoHistGlue            ( pEvoHistGlue  ),
    m_pDelay                  ( pDelay        ),
	m_pColorManager           ( pColorManager ),
	m_pStatusBar              ( pStatusBar    ),
	m_pGridWindow             ( pGridWindow   ),
	m_pEvoEditorWindow        ( pEditorWindow ),
	m_hCrsrWait               ( nullptr )
{ }

EvoController::~EvoController( )
{
	m_pAppWindow              = nullptr;
	m_pEvoWorkThreadInterface = nullptr;
	m_pWinManager             = nullptr;
	m_pEvoHistGlue            = nullptr;
	m_pColorManager           = nullptr;
	m_pDelay                  = nullptr;
    m_pStatusBar              = nullptr;
	m_pGridWindow             = nullptr;
	m_pEvoEditorWindow        = nullptr;
	m_hCrsrWait               = nullptr;
}

void EvoController::Initialize
( 
 	EvoAppWindow           * const pAppWindow,
	EvoWorkThreadInterface * const pEvoWorkThreadInterface
)
{
	m_pEvoWorkThreadInterface = pEvoWorkThreadInterface;
	m_pAppWindow              = pAppWindow;
	m_hCrsrWait               = LoadCursor( NULL, IDC_WAIT );
}

void EvoController::scriptDialog( )
{
	// TODO: replace by general solution
	wchar_t szBuffer[MAX_PATH];
	DWORD const dwRes = GetCurrentDirectory( MAX_PATH, szBuffer);
	assert( dwRes > 0 );
	wstring const wstrPath( szBuffer );
	wstring wstrFile = AskForScriptFileName( wstrPath );
	if ( ! wstrFile.empty( ) )
	{
		Stopwatch stopwatch;
		stopwatch.Start();
		std::wcout << L"Processing script file " << wstrFile << L"...";
		Script::ProcessScript( wstrFile );
		stopwatch.Stop( L"" );
	}
}

bool EvoController::ProcessUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{
	case IDM_DISP_WINDOW:
	case IDM_EDIT_WINDOW:
	case IDM_MAIN_WINDOW:
	case IDM_STAT_WINDOW:
	case IDM_CRSR_WINDOW:
	case IDM_HIST_INFO:
	case IDM_PERF_WINDOW:
	case IDM_MINI_WINDOW:
	case IDM_HIST_WINDOW:
		SendMessage( m_pWinManager->GetHWND( wmId ), WM_COMMAND, IDM_WINDOW_ON, 0 );
		break;

	case IDD_TOGGLE_STRIP_MODE:
		m_pGridWindow->ToggleStripMode();
		break;

	case IDD_TOGGLE_CLUT_MODE:
		m_pGridWindow->ToggleClutMode();
		break;

	case IDD_TOGGLE_COORD_DISPLAY:
		Config::SetConfigValueBoolOp( Config::tId::showGridPointCoords, tBoolOp::opToggle );
		m_pGridWindow->Refresh();
		break;

	case IDM_FIT_ZOOM:
		m_pGridWindow->Fit2Rect( );
		ZoomControl::SetSizeTrackBar( m_pStatusBar, m_pGridWindow->GetFieldSize() );
		break;
		
	case IDM_MAX_SPEED:
		SpeedControl::Set2MaxSpeed( );
		m_pDelay->SetDelay( 0 );
		break;

	case IDM_TRACKBAR:
		switch ( lParam )
		{
		case IDM_ZOOM_TRACKBAR:
		{
			LONG const lLogicalPos = m_pStatusBar->GetTrackBarPos( IDM_ZOOM_TRACKBAR );
			LONG const lValue      = lLogicalPos;
			LONG const lPos        = LogarithmicTrackbar::TrackBar2Value( lValue );
			ProcessUIcommand( IDM_ZOOM_TRACKBAR, lPos );
		}
		break;

		case IDM_SIMULATION_SPEED:
			m_pDelay->SetDelay( SpeedControl::GetTrackBarPos( ) );
			break;

		default:
			assert( false );
		}
		break;

	case IDD_EDITOR:
		ShowWindow( m_pStatusBar->GetDlgItem( IDM_EDIT_WINDOW ), ! m_pEvoEditorWindow->IsWindowVisible( ) );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pGridWindow->Zoom( wmId == IDM_ZOOM_IN );
		ZoomControl::SetSizeTrackBar( m_pStatusBar, m_pGridWindow->GetFieldSize() );
		break;

	case IDM_SET_ZOOM:
		m_pGridWindow->SetFieldSize( PIXEL(CastToShort(lParam)));
		ZoomControl::SetSizeTrackBar( m_pStatusBar, PIXEL(CastToShort(lParam)) );
		break;

	case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
		(void)m_pGridWindow->SetFieldSize( PIXEL(CastToShort(lParam)) );
		break;

	case IDM_REFRESH:
		m_pGridWindow->Refresh();
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

bool EvoController::ProcessModelCommand( int const wmId, LPARAM const lParam )
{
	switch ( wmId )
	{
	case IDM_RUN:
		m_pEvoEditorWindow->SendClick( IDM_MOVE );   // change edit mode to move
		SpeedControl::Adjust( TRUE, m_pEvoWorkThreadInterface );
		return true;

	case IDM_STOP:
		SpeedControl::Adjust( FALSE, m_pEvoWorkThreadInterface );
		return true;

	case IDM_RESET:
	{
		int     iRes    = ResetDialog::Show( m_pAppWindow->GetWindowHandle( ) );
		HCURSOR crsrOld = SetCursor( m_hCrsrWait );
		switch ( iRes )
		{
		case IDM_SOFT_RESET:
			m_pEvoWorkThreadInterface->PostReset( FALSE );
			break;

		case IDM_HISTORY_RESET:
			m_pEvoWorkThreadInterface->PostReset( TRUE );
			break;

		case IDM_HARD_RESET:
			m_pAppWindow->Stop();
			GridDimensions::DefineGridSize
			( 
				GRID_COORD( ResetDialog::GetNewWidth() ), 
				GRID_COORD( ResetDialog::GetNewHeight() ), 
				ResetDialog::GetNewNrOfNeighbors()
			);
			m_pAppWindow->Start();
		}
		SetCursor( crsrOld );
	}
	break;

	case IDM_SOFT_RESET:
		m_pEvoWorkThreadInterface->PostReset( FALSE );
		break;

	case IDM_HISTORY_RESET:
		m_pEvoWorkThreadInterface->PostReset( TRUE );
		break;

	case IDM_GOTO_ORIGIN:
		m_pEvoWorkThreadInterface->PostGotoOrigin( UnpackFromLParam(lParam) );
		break;

	case IDM_GOTO_DEATH:
		m_pEvoWorkThreadInterface->PostGotoDeath( UnpackFromLParam(lParam) );
		break;

	case IDM_SET_POI:
		m_pEvoWorkThreadInterface->PostSetPOI( UnpackFromLParam(lParam) );
		break;

	case IDM_SCRIPT_DIALOG:
		scriptDialog( );
		break;

	case IDM_ESCAPE:
		m_pGridWindow->Escape();
		break;

	default:
		return true;
	}

	return false;
}


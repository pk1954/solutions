// win32_evoController.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "config.h"
#include "GridDimensions.h"
#include "EvoHistorySysGlue.h"
#include "win32_aboutBox.h"
#include "win32_appWindow.h"
#include "win32_script.h"
#include "win32_stopwatch.h"
#include "win32_workThreadInterface.h"
#include "win32_winManager.h"
#include "win32_delay.h"
#include "win32_status.h"
#include "win32_editor.h"
#include "win32_appMenu.h"
#include "win32_gridWindow.h"
#include "win32_packGridPoint.h"
#include "win32_evoController.h"
#include "win32_colorManager.h"
#include "win32_resetDlg.h"

EvoController::EvoController() :
    m_bTrace               ( TRUE ),
    m_pTraceStream         ( nullptr ),
	m_pWorkThreadInterface ( nullptr ),
	m_pWinManager          ( nullptr ),
	m_pEvoHistGlue         ( nullptr ),
    m_pDelay               ( nullptr ),
	m_pColorManager        ( nullptr ),
	m_pStatusBar           ( nullptr ),
	m_pGridWindow          ( nullptr ),
	m_pEditorWindow        ( nullptr ),
	m_pAppMenu             ( nullptr )
{ }

EvoController::~EvoController( )
{
    m_pTraceStream         = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pWinManager          = nullptr;
	m_pEvoHistGlue         = nullptr;
	m_pColorManager        = nullptr;
	m_pDelay               = nullptr;
    m_pStatusBar           = nullptr;
	m_pGridWindow          = nullptr;
	m_pEditorWindow        = nullptr;
	m_pAppMenu             = nullptr;
}

void EvoController::Initialize
( 
 	AppWindow           * const pAppwindow,
    std::wostream       *       pTraceStream,
	WorkThreadInterface * const pWorkThreadInterface,
	WinManager          * const pWinManager,
	EvoHistorySysGlue   * const pEvoHistGlue,
	Delay               * const pDelay,
	ColorManager        * const pColorManager,
	AppMenu             * const pAppMenu,
	StatusBar           * const pStatusBar,
	GridWindow          * const pGridWindow,
	EditorWindow        * const pEditorWindow
)
{
	m_pTraceStream         = pTraceStream;
	m_pAppWindow           = pAppwindow;
	m_pWorkThreadInterface = pWorkThreadInterface;
	m_pWinManager          = pWinManager;
	m_pEvoHistGlue         = pEvoHistGlue;
    m_pDelay               = pDelay;
	m_pStatusBar           = pStatusBar;
	m_pGridWindow          = pGridWindow;
	m_pEditorWindow        = pEditorWindow;
	m_pColorManager        = pColorManager;
	m_pAppMenu             = pAppMenu;
	m_hCrsrWait            = LoadCursor( NULL, IDC_WAIT );
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

bool EvoController::processUIcommand( int const wmId, LPARAM const lParam )
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
		m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pGridWindow->Zoom( wmId == IDM_ZOOM_IN );
		m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
		break;

	case IDM_SET_ZOOM:
		m_pGridWindow->SetFieldSize( PIXEL(CastToShort(lParam)));
		m_pStatusBar->SetSizeTrackBar( PIXEL(CastToShort(lParam)) );
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

void EvoController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
	
	if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                             // but do not concern the model

    switch (wmId)
    {
		case IDM_ABOUT:
			ShowAboutBox( m_pAppWindow->GetWindowHandle( ) );
			break;

		case IDM_EDIT_UNDO:
			m_pWorkThreadInterface->PostUndo( );
			break;

        case IDM_EDIT_REDO:
 			m_pWorkThreadInterface->PostRedo( );
			break;

	    case IDM_GENERATION:
            m_pWorkThreadInterface->PostGenerationStep( );
            break;

		case IDM_RUN:
			m_pEditorWindow->SendClick( IDM_MOVE );   // change edit mode to move
			m_pWorkThreadInterface->PostRunGenerations( true );
			break;

		case IDM_STOP:
            m_pWorkThreadInterface->PostStopComputation( );
			break;

		case IDM_HIST_BUFFER_FULL:
			std::wcout << L"History buffer is full" << std::endl;
			(void)MessageBeep( MB_ICONWARNING );
			ProcessCommand( IDM_STOP );
			break;

		case IDM_RESET:
		{
			int     iRes    = ResetDialog::Show( m_pAppWindow->GetWindowHandle( ) );
			HCURSOR crsrOld = SetCursor( m_hCrsrWait );
			switch ( iRes )
			{
			case IDM_SOFT_RESET:
				m_pWorkThreadInterface->PostReset( FALSE );
				break;

			case IDM_HISTORY_RESET:
				m_pWorkThreadInterface->PostReset( TRUE );
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
            m_pWorkThreadInterface->PostReset( FALSE );
            break;

        case IDM_HISTORY_RESET:
            m_pWorkThreadInterface->PostReset( TRUE );
            break;

		case IDM_BACKWARDS:
			m_pWorkThreadInterface->PostPrevGeneration( );
			break;

		case IDM_GOTO_ORIGIN:
			m_pWorkThreadInterface->PostGotoOrigin( UnpackFromLParam(lParam) );
			break;

		case IDM_GOTO_DEATH:
			m_pWorkThreadInterface->PostGotoDeath( UnpackFromLParam(lParam) );
			break;

		case IDM_SET_POI:
			m_pWorkThreadInterface->PostSetPOI( UnpackFromLParam(lParam) );
			break;

        case IDM_SCRIPT_DIALOG:
			scriptDialog( );
			break;

        case IDM_ESCAPE:
			m_pGridWindow->Escape();
            break;

		case IDM_EXIT:
			PostMessage (m_pAppWindow->GetWindowHandle( ),  WM_CLOSE, 0, 0 );
			break;

		default:
			assert( false );
	        break;
    }
}

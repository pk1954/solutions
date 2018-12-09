// win32_evoController.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Resource.h"
#include "BoolOp.h"
#include "config.h"
#include "win32_script.h"
#include "win32_workThreadInterface.h"
#include "win32_winManager.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"
#include "win32_editor.h"
#include "win32_gridWindow.h"
#include "win32_packGridPoint.h"
#include "win32_evoController.h"
#include "win32_colorManager.h"

EvoController::EvoController() :
	m_bSimulationMode      ( FALSE ),
    m_bTrace               ( TRUE ),
    m_pTraceStream         ( nullptr ),
	m_pWorkThreadInterface ( nullptr ),
	m_pWinManager          ( nullptr ),
    m_pPerformanceWindow   ( nullptr ),
    m_pStatusBar           ( nullptr ),
    m_pGridWindow          ( nullptr ),
	m_pEditorWindow        ( nullptr )
{ }

EvoController::~EvoController( )
{
    m_pTraceStream         = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pWinManager          = nullptr;
    m_pPerformanceWindow   = nullptr;
    m_pStatusBar           = nullptr;
	m_pGridWindow          = nullptr;
	m_pEditorWindow        = nullptr;
}

void EvoController::Start
( 
    wostream            *       pTraceStream,
	WorkThreadInterface * const pWorkThreadInterface,
	WinManager          * const pWinManager,
    PerformanceWindow   * const pPerformanceWindow,
	StatusBar           * const pStatusBar,
	GridWindow          * const pGridWindow,
	EditorWindow        * const pEditorWindow,
	ColorManager        * const pColorManager
)
{
	m_pTraceStream         = pTraceStream;
	m_pWorkThreadInterface = pWorkThreadInterface;
	m_pWinManager          = pWinManager;
    m_pPerformanceWindow   = pPerformanceWindow;
	m_pStatusBar           = pStatusBar;
	m_pGridWindow          = pGridWindow;
	m_pEditorWindow        = pEditorWindow;
	m_pColorManager        = pColorManager;
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
		m_pWorkThreadInterface->PostProcessScript( wstrFile );
}

void EvoController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
    switch (wmId)
    {
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
            m_pWinManager->Show( IDM_HIST_WINDOW, tBoolOp::opFalse );
			m_pWorkThreadInterface->PostRunGenerations( );
			m_pStatusBar->SetRunMode( TRUE );
			break;

		case IDM_STOP:
            m_pWorkThreadInterface->PostStopComputation( );
            m_pWinManager->Show( IDM_HIST_WINDOW, tBoolOp::opTrue );
			m_pStatusBar->SetRunMode( FALSE );
            break;

		case IDM_RUN_STOP:
			ProcessCommand( m_pWorkThreadInterface->IsRunning() ? IDM_STOP : IDM_RUN, 0 );
			break;

		case IDM_HIST_BUFFER_FULL:
			wcout << L"History buffer is full" << endl;
			(void)MessageBeep( MB_ICONWARNING );
			ProcessCommand( IDM_STOP, 0 );
			break;

        case IDM_RESET:
            m_pWorkThreadInterface->PostReset( FALSE );
            break;

        case IDM_RESTART:
            m_pWorkThreadInterface->PostReset( TRUE );
            break;

		case IDM_BACKWARDS:
			m_pWorkThreadInterface->PostPrevGeneration( );
			break;

		case IDM_GOTO_ORIGIN:
		case IDM_GOTO_DEATH:
			m_pWorkThreadInterface->PostHistoryAction( wmId, UnpackFromLParam(lParam) );
			break;

		case IDM_SIMULATION_SPEED:   // comes from trackbar in statusBar
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->SetPerfGenerationDelay( static_cast<DWORD>( lParam ) );
            break;

		case IDM_MAX_SPEED:
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->SetPerfGenerationDelay( static_cast<DWORD>( lParam ) );
			if (m_pStatusBar != nullptr)
				m_pStatusBar->SetSpeedTrackBar( 0 );
            break;

        case IDM_SCRIPT_DIALOG:
			scriptDialog( );
            break;
                 
		case IDM_MINI_WINDOW_ON:
		case IDM_MINI_WINDOW_OFF:
		case IDM_MINI_WINDOW_AUTO:
			m_pGridWindow->PostCommand2Application( IDM_ADJUST_MINI_WIN, wmId );
			break;

        case IDM_DISP_WINDOW:
        case IDM_EDIT_WINDOW:
        case IDM_STAT_WINDOW:
        case IDM_HIST_WINDOW:
        case IDM_CRSR_WINDOW:
		case IDM_PERF_WINDOW:
		case IDM_HIST_INFO:
            m_pWinManager->Show( wmId, tBoolOp::opToggle );
            break;

		case IDM_SHOW_PERF_WINDOW:
            m_pWinManager->Show( IDM_PERF_WINDOW, BoolOp(static_cast<bool>(lParam)) );
            break;

        case IDM_REFRESH:
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDD_TOGGLE_STRIP_MODE:
			m_pGridWindow->ToggleStripMode();
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDD_TOGGLE_CLUT_MODE:
			m_pColorManager->ToggleClutMode();
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDD_TOGGLE_COORD_DISPLAY:
			Config::SetConfigValueBoolOp( Config::tId::showGridPointCoords, tBoolOp::opToggle );
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDM_ESCAPE:
			m_pGridWindow->Escape();
            break;

        case IDM_FIT_ZOOM:
			m_pGridWindow->Fit2Rect( );
			m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDM_ZOOM_OUT:
        case IDM_ZOOM_IN:
            m_pGridWindow->Zoom( wmId == IDM_ZOOM_IN );
			m_pStatusBar->SetSizeTrackBar( m_pGridWindow->GetFieldSize() );
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

		case IDM_SET_ZOOM:
            (void)m_pGridWindow->SetFieldSize( static_cast<SHORT>(lParam));
			m_pStatusBar->SetSizeTrackBar( static_cast<SHORT>(lParam) );
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
            (void)m_pGridWindow->SetFieldSize( static_cast<SHORT>(lParam) );
			m_pWorkThreadInterface->PostRefresh( lParam );
            break;

        case IDM_TOGGLE_SIMU_MODE:
			m_pWorkThreadInterface->PostSetSimulationMode( tBoolOp::opToggle );
            break;

        case IDM_SET_SIMU_MODE:
			m_pWorkThreadInterface->PostSetSimulationMode( static_cast<tBoolOp>(lParam) );
            break;

		case IDM_SET_POI:
			m_pWorkThreadInterface->PostSetPOI( UnpackFromLParam(lParam) );
			break;

		default:
			assert( false );
	        break;
    }
}

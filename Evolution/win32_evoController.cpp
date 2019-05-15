// win32_evoController.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Resource.h"
#include "BoolOp.h"
#include "config.h"
#include "EvoHistorySysGlue.h"
#include "win32_script.h"
#include "win32_stopwatch.h"
#include "win32_workThreadInterface.h"
#include "win32_ViewCollection.h"
#include "win32_winManager.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"
#include "win32_editor.h"
#include "win32_appMenu.h"
#include "win32_gridWindow.h"
#include "win32_packGridPoint.h"
#include "win32_evoController.h"
#include "win32_colorManager.h"

EvoController::EvoController() :
    m_bTrace               ( TRUE ),
    m_pTraceStream         ( nullptr ),
	m_pWorkThreadInterface ( nullptr ),
	m_pWinManager          ( nullptr ),
	m_pEvoHistGlue         ( nullptr ),
    m_pPerformanceWindow   ( nullptr ),
	m_pCoreObservers       ( nullptr ),
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
	m_pCoreObservers       = nullptr;
	m_pColorManager        = nullptr;
	m_pPerformanceWindow   = nullptr;
    m_pStatusBar           = nullptr;
	m_pGridWindow          = nullptr;
	m_pEditorWindow        = nullptr;
	m_pAppMenu             = nullptr;
}

void EvoController::Start
( 
    std::wostream       *       pTraceStream,
	WorkThreadInterface * const pWorkThreadInterface,
	ViewCollection      * const pCoreObservers,
	WinManager          * const pWinManager,
	EvoHistorySysGlue   * const pEvoHistGlue,
	PerformanceWindow   * const pPerformanceWindow,
	StatusBar           * const pStatusBar,
	GridWindow          * const pGridWindow,
	EditorWindow        * const pEditorWindow,
	ColorManager        * const pColorManager,
	AppMenu             * const pAppMenu
)
{
	m_pTraceStream         = pTraceStream;
	m_pWorkThreadInterface = pWorkThreadInterface;
	m_pCoreObservers       = pCoreObservers;
	m_pWinManager          = pWinManager;
	m_pEvoHistGlue         = pEvoHistGlue;
    m_pPerformanceWindow   = pPerformanceWindow;
	m_pStatusBar           = pStatusBar;
	m_pGridWindow          = pGridWindow;
	m_pEditorWindow        = pEditorWindow;
	m_pColorManager        = pColorManager;
	m_pAppMenu             = pAppMenu;
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

void EvoController::SetSimulationMode( bool const bSimulationMode )  	// adjust window configuration according to simulation or edit mode
{
	m_pStatusBar->SetSimuMode( bSimulationMode );
	m_pEditorWindow->SetSimuMode( bSimulationMode );

	if ( ! bSimulationMode )
		m_pStatusBar->SetRunMode( FALSE );

	m_pWinManager->Show( IDM_PERF_WINDOW, BoolOp( bSimulationMode ) );
}

void EvoController::enterEditMode( )
{
	HIST_GENERATION genCurrent  = m_pEvoHistGlue->GetCurrentGeneration( );
	HIST_GENERATION genYoungest = m_pEvoHistGlue->GetYoungestGeneration( );
	if ( genCurrent < genYoungest )              // in history mode ?
	{
		std::wostringstream wBuffer;
		wBuffer << L"Gen " << ( genCurrent + 1 ) << L" - " << genYoungest << L" will be deleted.";
		int iRes = MessageBox( nullptr, L"Cut off history?", wBuffer.str( ).c_str( ), MB_OKCANCEL | MB_SYSTEMMODAL );
		if ( iRes == IDOK ) 
			m_pEvoHistGlue->EvoClearHistory( genCurrent );  // cut off future generations
		else 
			return;            // user answered no, cancel operation
	}

	ProcessCommand( IDM_STOP );
	SetSimulationMode( false );
}

bool EvoController::processUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{
	case IDD_TOGGLE_STRIP_MODE:
		m_pGridWindow->ToggleStripMode();
		break;

	case IDD_TOGGLE_CLUT_MODE:
		m_pColorManager->ToggleClutMode();
		break;

	case IDD_TOGGLE_COORD_DISPLAY:
		Config::SetConfigValueBoolOp( Config::tId::showGridPointCoords, tBoolOp::opToggle );
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
		break;

	default:
		return FALSE; // command has not been processed
	}

	m_pCoreObservers->Notify( TRUE );
	return TRUE;  // command has been processed
}

void EvoController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
	
	if ( processUIcommand( wmId, lParam ) ) // handle all commands that affect the UI
		return;                             // but do not concern the model

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
			SetSimulationMode( true );
			m_pWorkThreadInterface->PostRunGenerations( true );
			m_pAppMenu->RunMode( TRUE );
			m_pWinManager->Show( IDM_HIST_WINDOW, tBoolOp::opFalse );
			m_pStatusBar->SetRunMode( TRUE );
			break;

		case IDM_STOP:
            m_pWorkThreadInterface->PostStopComputation( );
			m_pAppMenu->RunMode( FALSE );
			m_pWinManager->Show( IDM_HIST_WINDOW, tBoolOp::opTrue );
			m_pStatusBar->SetRunMode( FALSE );
			break;

		case IDM_RUN_STOP:
			ProcessCommand( m_pWorkThreadInterface->IsRunning() ? IDM_STOP : IDM_RUN );
			break;

		case IDM_HIST_BUFFER_FULL:
			std::wcout << L"History buffer is full" << std::endl;
			(void)MessageBeep( MB_ICONWARNING );
			ProcessCommand( IDM_STOP );
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

		case IDM_EDIT_MODE:
			enterEditMode( );
			break;

		case IDM_SIMU_MODE:
			SetSimulationMode( true );
			break;

		case IDM_SET_POI:
			m_pWorkThreadInterface->PostSetPOI( UnpackFromLParam(lParam) );
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

        case IDM_ESCAPE:
			m_pGridWindow->Escape();
            break;

		default:
			assert( false );
	        break;
    }
}

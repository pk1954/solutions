// EvoController.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Resource.h"
#include "win32_script.h"
#include "win32_histWorkerThread.h"
#include "win32_winManager.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"
#include "win32_gridWindow.h"
#include "EvoController.h"

EvoController::EvoController() :
    m_bTrace             ( TRUE ),
    m_pTraceStream       ( nullptr ),
	m_pHistWorkThread    ( nullptr ),
	m_pWinManager        ( nullptr ),
    m_pPerformanceWindow ( nullptr ),
    m_pStatusBar         ( nullptr ),
    m_pGridWindow        ( nullptr )
{ }

EvoController::~EvoController( )
{
    m_pTraceStream       = nullptr;
	m_pHistWorkThread    = nullptr;
	m_pWinManager        = nullptr;
    m_pPerformanceWindow = nullptr;
    m_pStatusBar         = nullptr;
	m_pGridWindow        = nullptr;
}

void EvoController::Start
( 
    wostream          *       pTraceStream,
	HistWorkThread    * const pHistWorkThread,
	WinManager        * const pWinManager,
    PerformanceWindow * const pPerformanceWindow,
	StatusBar         * const pStatusBar,
	GridWindow        * const pGridWindow

)
{
	m_pTraceStream       = pTraceStream;
	m_pHistWorkThread    = pHistWorkThread;
	m_pWinManager        = pWinManager;
    m_pPerformanceWindow = pPerformanceWindow;
	m_pStatusBar         = pStatusBar;
	m_pGridWindow        = pGridWindow;
}

void EvoController::SetGenerationDelay( DWORD const dwNewDelay )  // in milliseconds
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << dwNewDelay << endl;
    m_pPerformanceWindow->SetGenerationDelay( dwNewDelay );
    m_pStatusBar->SetSpeedTrackBar( dwNewDelay );
}

void EvoController::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
    int const wmId = LOWORD( wParam );
    switch (wmId)
    {
        case IDM_GENERATION:
            m_pHistWorkThread->PostGenerationStep( );
            break;

		case IDM_RUN:
			m_pHistWorkThread->PostRunGenerations( );
			break;

		case IDM_STOP:
            m_pHistWorkThread->PostStopComputation( );
            break;

        case IDM_RESET:
            m_pHistWorkThread->PostReset( );
            break;

        case IDM_BACKWARDS:
			m_pHistWorkThread->PostPrevGeneration( );
			break;

		case IDM_GOTO_ORIGIN:
		case IDM_GOTO_DEATH:
			m_pHistWorkThread->PostHistoryAction( wmId, GridPoint( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) );
			break;

		case IDM_MAX_SPEED:
            SetGenerationDelay( 0 );
            break;

        case IDM_SCRIPT_DIALOG:
            {
                // TODO: replace by general solution
                wchar_t szBuffer[MAX_PATH];
                DWORD const dwRes = GetCurrentDirectory( MAX_PATH, szBuffer);
                assert( dwRes > 0 );
                wstring const wstrPath( szBuffer );
                wstring wstrFile = OpenScriptFile( wstrPath );
                if ( ! wstrFile.empty( ) )
                   m_pHistWorkThread->PostProcessScript( wstrFile );
            }
            break;
                 
        case IDM_ESCAPE:
            m_pGridWindow->ResetSelection( );
            break;

		case IDM_MINI_WINDOW:
        case IDM_DISP_WINDOW:
        case IDM_EDIT_WINDOW:
        case IDM_STAT_WINDOW:
        case IDM_HIST_WINDOW:
        case IDM_CRSR_WINDOW:
        case IDM_PERF_WINDOW:
            m_pWinManager->Show( wmId, tBoolOp::opToggle );
            break;

        case IDD_TOGGLE_STRIP_MODE:
        case IDD_TOGGLE_CLUT_MODE:
        case IDM_ZOOM_OUT:
        case IDM_ZOOM_IN:
        case IDM_SET_ZOOM:
        case IDM_FIT_ZOOM:
            (void)m_pGridWindow->SendMessage( WM_COMMAND, wParam, lParam );
            break;

		default:
	        break;
    }
}


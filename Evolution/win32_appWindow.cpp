// win32_appWindow.cpp
//

//lint -e1924                // C-style cast
//lint -e613                 // usage of nullptr
//   lint -esym( 715, script )  // not referenced

#include "stdafx.h"
#include "commctrl.h"

// EvolutionCore interfaces

#include "config.h"
#include "gridRect.h"
#include "wrappers.h"
#include "ModelData.h"
#include "EvolutionCore.h"

// history system

#include "EvoHistorySys.h"
#include "win32_histWorkerThread.h"

// DEBUG only
#include "win32_frameBuffer.h"

// interfaces of various windows

#include "win32_gridWindow.h"
#include "win32_evoHistWindow.h"
#include "win32_editor.h"
#include "win32_displayOptions.h"
#include "win32_appWindow.h"
#include "win32_status.h"
#include "win32_statistics.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_aboutBox.h"

// infrastructure

#include "win32_winManager.h"
#include "win32_worker_thread.h"
#include "win32_focusPoint.h"
#include "win32_displayAll.h"

// scripting and tracing

#include "script.h"
#include "errhndl.h"
#include "win32_wrappers.h"
#include "win32_script.h"
#include "win32_editorWrappers.h"
#include "win32_histWrappers.h"

// system and resources

#include "d3d_system.h"
#include "Resource.h"

// application

#include "EvoModelData.h"
#include "EvoNextGenFunctor.h"

#include "win32_appWindow.h"

class ScriptHook : public Script_Functor
{
public:

    explicit ScriptHook( StatusBar * const pStatusBar ) : m_pStatusBar( pStatusBar ) {};

    virtual void operator() ( Script & script ) const
    {
        if ( m_pStatusBar != nullptr )
        {
            wstring const & wszPath = script.GetActPath( );
            wstring const & wszLine = script.GetActLine( );
            int     const   iLineNr = script.GetActLineNr( );
            m_pStatusBar->DisplayScriptLine( wszPath, iLineNr, wszLine );
        }
    }

private:

    StatusBar * m_pStatusBar;
};

//lint -esym( 715, hInstance )  // not referenced

AppWindow::AppWindow( HINSTANCE const hInstance )
:
    BaseWindow( ),
    m_displayGridFunctor( ),
    m_pMainGridWindow( nullptr ),
    m_pMiniGridWindow( nullptr ),
    m_pWorkThread( nullptr ),
    m_pPerfWindow( nullptr ),
    m_pEditorWindow( nullptr ),
    m_pCrsrWindow( nullptr ),
    m_pStatusBar( nullptr ),
    m_pStatistics( nullptr ),
    m_pDspOptWindow( nullptr ),
    m_pGridRectSel( nullptr ),
    m_pFocusPoint( nullptr ),
    m_pWinManager( nullptr ),
    m_pEvoModelWork( nullptr ),
    m_pModelWork( nullptr ),
    m_pEvolutionCore( nullptr ),
    m_pScriptHook( nullptr ),
    m_pEvoNextGenFunctor( nullptr ),
    m_pEvoHistWindow( nullptr ),
    m_pHistWorkThread( nullptr ),

    m_traceStream( )
{};

void AppWindow::Start( LPTSTR lpCmdLine )
{
    HWND      const hwnd       = StartBaseWindow( nullptr, CS_HREDRAW | CS_VREDRAW, L"ClassAppWindow", WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN );
    HINSTANCE const hInstance  = GetModuleHandle( nullptr );
    HICON     const hIconBig   = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_EVOLUTION ) );
    HICON     const hIconSmall = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    SendMessage( WM_SETICON, ICON_BIG,   (LPARAM)hIconBig   );
    SendMessage( WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall );

    m_traceStream.open( L"main_trace.out", ios::out );
    assert( m_traceStream.good( ) );

    InitCommonControls( ); // loads common control's DLL 
    
    Config::InitializeConfig( );
    EvolutionCore::InitClass( );

    m_pEvolutionCore = EvolutionCore::CreateCore( );

    ScriptErrorHandler::ScrSetOutputStream( & m_traceStream );

	ProcessScript( L"std_configuration.in" );

    D3dSystem::Create( hwnd, GridPoint::GRID_WIDTH, GridPoint::GRID_HEIGHT );

    // evaluate command line parameters

    std::wstring const wstrCmdLine( lpCmdLine );
    if ( wstrCmdLine.compare( L"/nohist" ) == 0 )
        Config::SetConfigValue( Config::tId::maxGeneration, 0 );

    // create window objects

    m_pGridRectSel    = new GridRect( GridRect::GRID_RECT_EMPTY );  //
    m_pFocusPoint     = new FocusPoint( );                          //
    m_pWinManager     = new WinManager( );                          //
    m_pStatusBar      = new StatusBar( );                           //
    m_pCrsrWindow     = new CrsrWindow( );                          //
    m_pDspOptWindow   = new DspOptWindow( );                        //
    m_pEditorWindow   = new EditorWindow( );                        //
    m_pStatistics     = new StatisticsWindow( );                    //
    m_pMainGridWindow = new GridWindow( );                          //
    m_pMiniGridWindow = new GridWindow( );                          //
    m_pPerfWindow     = new PerformanceWindow( );                   //

    SetMenu( hwnd, LoadMenu( hInstance, MAKEINTRESOURCE( IDC_EVOLUTION_MAIN ) ) );
    {
        static int   const MAX_LOADSTRING = 100;
        static TCHAR szTitle[ MAX_LOADSTRING ];			// Titelleistentext
        (void)LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
        SetWindowText( hwnd, szTitle );
    }

    m_pModelWork = ModelData::CreateModelData( );
    DefineModelWrapperFunctions( m_pModelWork );

    if ( Config::UseHistorySystem( ) )
    {
        m_pEvoModelWork      = new EvoModelData( m_pModelWork );
        m_pEvoHistWindow     = new EvoHistWindow( );
        m_pEvoNextGenFunctor = new EvoNextGenFunctor( );
        m_pEvoHistWindow->Start( hwnd, & m_traceStream, m_pFocusPoint, m_pStatusBar, m_pEvoNextGenFunctor, m_pEvoModelWork, m_pEvolutionCore );
        m_pWorkThread = m_pEvoHistWindow->GetHistWorkThread( );
        m_pEvoNextGenFunctor->Start( m_pWorkThread );
        m_pWorkThread->Start( m_pStatusBar, m_pEditorWindow, m_pPerfWindow, & m_displayGridFunctor, m_pEvolutionCore, m_pModelWork );

        m_pWinManager->AddWindow( L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, m_pEvoHistWindow, 75 );

        DefineWin32HistWrapperFunctions( m_pEvoHistWindow->GetHistWorkThread( ) );
    }
    else
    {
        m_pWorkThread = new WorkThread( & m_traceStream );
        EnableMenuItem( GetMenu( hwnd ), IDM_BACKWARDS, MF_GRAYED );
        m_pWorkThread->Start( m_pStatusBar, m_pEditorWindow, m_pPerfWindow, & m_displayGridFunctor, m_pEvolutionCore, m_pModelWork );
    }

    m_pDspOptWindow  ->Start( hwnd, m_pWorkThread, m_pModelWork );
    m_pEditorWindow  ->Start( hwnd, m_pWorkThread, m_pModelWork, m_pDspOptWindow );
    m_pStatusBar     ->Start( hwnd, m_pWorkThread, m_pModelWork );
    m_pMainGridWindow->Start( hwnd, m_pWorkThread, m_pGridRectSel, m_pEditorWindow, m_pFocusPoint, m_pDspOptWindow, m_pPerfWindow, m_pStatusBar, m_pEvolutionCore, m_pModelWork, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 16 );
    m_pMiniGridWindow->Start( hwnd, m_pWorkThread, m_pGridRectSel, m_pEditorWindow, m_pFocusPoint, m_pDspOptWindow, m_pPerfWindow, m_pStatusBar, m_pEvolutionCore, m_pModelWork, WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CAPTION, 1 );
    m_pStatistics    ->Start( hwnd, m_pModelWork, m_pGridRectSel );
    m_pCrsrWindow    ->Start( hwnd, m_pFocusPoint, m_pModelWork );
    m_pPerfWindow    ->Start( hwnd, 100 );

    m_pWinManager->AddWindow( L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, this,               -1 );
    m_pWinManager->AddWindow( L"IDM_DISP_WINDOW", IDM_DISP_WINDOW, m_pDspOptWindow,    -1 );
    m_pWinManager->AddWindow( L"IDM_EDIT_WINDOW", IDM_EDIT_WINDOW, m_pEditorWindow,    -1 );
    m_pWinManager->AddWindow( L"IDM_CRSR_WINDOW", IDM_CRSR_WINDOW, m_pCrsrWindow,     500 );
    m_pWinManager->AddWindow( L"IDM_STAT_WINDOW", IDM_STAT_WINDOW, m_pStatistics,     500 );
    m_pWinManager->AddWindow( L"IDM_PERF_WINDOW", IDM_PERF_WINDOW, m_pPerfWindow,     500 );
    m_pWinManager->AddWindow( L"IDM_MINI_WINDOW", IDM_MINI_WINDOW, m_pMiniGridWindow, 300 );
    m_pWinManager->AddWindow( L"IDM_MAIN_WINDOW", IDM_MAIN_WINDOW, m_pMainGridWindow, 100 );
    m_pWinManager->AddWindow( L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_pStatusBar,       -1 );

    m_pMiniGridWindow->Observe( m_pMainGridWindow );
    m_pMiniGridWindow->Size( );

    m_displayGridFunctor.SetWinManager( m_pWinManager );

    m_pEvolutionCore->SetGridDisplayFunctor( & m_displayGridFunctor );   // display callback for core

    m_pScriptHook = new ScriptHook( m_pStatusBar );
    Script::ScrSetWrapHook( m_pScriptHook );

    DefineWin32WrapperFunctions( m_pWorkThread, m_pStatusBar );
    DefineWin32EditorWrapperFunctions( m_pEditorWindow );

    CheckMenuItem( GetMenu( hwnd ), IDM_STAT_WINDOW, MF_CHECKED );

    m_pWinManager->GetWindowConfiguration( );

    ShowWindow( hwnd, SW_SHOWMAXIMIZED );
    (void)m_pMainGridWindow->SendMessage( WM_COMMAND, IDM_FIT_ZOOM, 0 );

	ProcessScript( L"std_script.in" );
}

AppWindow::~AppWindow( )
{
    try
    {
        if ( Config::UseHistorySystem( ) )
        {
            delete m_pEvoHistWindow;
            delete m_pEvoModelWork;
            delete m_pEvoNextGenFunctor;
        }
        else
        {
            delete m_pModelWork;
            delete m_pWorkThread;
        }

        delete m_pMiniGridWindow;  //
        delete m_pMainGridWindow;  //
        delete m_pStatusBar;  //
        delete m_pStatistics;  //
        delete m_pPerfWindow;  //
        delete m_pCrsrWindow;   //
        delete m_pEditorWindow;  //
        delete m_pDspOptWindow;  //
        delete m_pGridRectSel;  //
        delete m_pFocusPoint;  //
        delete m_pWinManager;  //
        delete m_pScriptHook;     //

        EvolutionCore::DeleteCore( );
    }
    catch ( ... )
    {
        exit( 1 );
    };
}

LRESULT AppWindow::UserProc
( 
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    switch (message)
    {

    case WM_COMMAND:
    {
        int const wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            ShowAboutBox( GetWindowHandle( ) );
            break;
        case IDM_GENERATION:
            m_pWorkThread->PostNextGeneration( );
            break;
        case IDM_BACKWARDS:
        case IDM_RUN:
        case IDM_STOP:
            m_pWorkThread->PostHistoryAction( wmId );
            break;
        case IDM_MAX_SPEED:
            m_pWorkThread->SetGenerationDelay( 0 );
            m_pStatusBar->SetSpeedTrackBar( 0 );
            break;
        case IDM_ESCAPE:
            m_pMainGridWindow->ResetSelection( );
            break;
        case IDD_TOGGLE_STRIP_MODE:
        case IDD_TOGGLE_CLUT_MODE:
        case IDM_ZOOM_OUT:
        case IDM_ZOOM_IN:
        case IDM_SET_ZOOM:
        case IDM_FIT_ZOOM:
            (void)m_pMainGridWindow->SendMessage( message, wParam, lParam );
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
                   m_pWorkThread->PostProcessScript( wstrFile );
            }
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

        case IDM_RESET:
            m_pWorkThread->PostReset( );
            break;

        case IDM_EXIT:
            PostMessage( WM_CLOSE, 0, 0 );
            break;

        default:
            break;
        }
    }
    break;

    case WM_SIZE:
    case WM_MOVE:
        adjustChildWindows( );
        break;

    case WM_CLOSE:
        m_pWinManager->StoreWindowConfiguration( );
        m_pWorkThread->PostEndThread( GetWindowHandle( ) );
        return 1;  // Do not call DefWindowProc. Worker thread will call DestroyWindow. 

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

    default:
        break;
    }
    
    return DefWindowProc( message, wParam, lParam );
}

void AppWindow::adjustChildWindows( )
{
    static int const HIST_WINDOW_HEIGHT = 30;

    PixelRectSize pntSize( GetClRectSize( ) );

    if ( ! pntSize.IsEmpty( ) )
    {
        m_pStatusBar->Resize( );
        pntSize.ReduceHeight( m_pStatusBar->GetHeight( ) );

        if ( m_pEvoHistWindow != nullptr )
        {
            PixelPoint pnt( 0, pntSize.GetHeight( ) - HIST_WINDOW_HEIGHT );  // client area coordinates
            ClientToScreen( GetWindowHandle( ), & pnt );
            m_pEvoHistWindow->Move( pnt.x, pnt.y, pntSize.GetWidth(), HIST_WINDOW_HEIGHT, TRUE );  // adapt history window to new size
            pntSize.ReduceHeight( HIST_WINDOW_HEIGHT );
        }

        m_pMainGridWindow->Move( 0, 0, pntSize.GetWidth( ), pntSize.GetHeight( ), TRUE );
    }
}

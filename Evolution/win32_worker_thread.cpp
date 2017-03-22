// win32_worker_thread.cpp
//

#include "stdafx.h"
#include "gridRect.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvoGenerationCmd.h"
#include "win32_hiResTimer.h"
#include "win32_script.h"
#include "win32_status.h"
#include "win32_performanceWindow.h"
#include "win32_displayAll.h"
#include "win32_worker_thread.h"

using namespace std;

//lint -e849    same enumerator value          
//lint -esym( 488, tThreadMessages::THREAD_MSG_LAST )  same value as THREAD_MSG_EXIT

WorkThread::WorkThread( wostream * pTraceStream ) :
    m_hEventThreadStarter( nullptr ),
    m_dwThreadId         ( 0 ),
    m_bTrace             ( TRUE ),
    m_gridRectSelection  ( GridRect::GRID_RECT_FULL ),
    m_pTraceStream       ( pTraceStream ),
    m_hTimer             ( nullptr ),
    m_iScriptLevel       ( 0 ),
    m_pStatusBar         ( nullptr ),
    m_pEditorWindow      ( nullptr ),
    m_pPerformanceWindow ( nullptr ),
    m_pDisplayGridFunctor( nullptr ),
    m_pEvolutionCore     ( nullptr ),
    m_pModelWork         ( nullptr ),
    m_bContinue          ( FALSE )
{ }

void WorkThread::Start
( 
    StatusBar          * const pStatus, 
    EditorWindow       * const pEditorWindow,
    PerformanceWindow  * const pPerformanceWindow,
    DisplayAll   const * const pDisplayGridFunctor,
    EvolutionCore      * const pEvolutionCore,
    EvolutionModelData * const pModel
)
{
    HANDLE const hThread  = Util::MakeThread( WorkerThread, this, &m_dwThreadId, &m_hEventThreadStarter );
    m_pEditorWindow       = pEditorWindow;
    m_pPerformanceWindow  = pPerformanceWindow;
    m_pStatusBar          = pStatus;
    m_pDisplayGridFunctor = pDisplayGridFunctor;
    m_pEvolutionCore      = pEvolutionCore;
    m_pModelWork          = pModel;

    (void)SetThreadAffinityMask( hThread, 0x0002 );
}

WorkThread::~WorkThread( )
{
    m_hEventThreadStarter = nullptr;
    m_pTraceStream        = nullptr;
    m_hTimer              = nullptr;
    m_pStatusBar          = nullptr;
    m_pEditorWindow       = nullptr;
    m_pPerformanceWindow  = nullptr;
    m_pDisplayGridFunctor = nullptr;
}

void WorkThread::ResetModel( )
{
    m_pEvolutionCore->ResetModel( m_pModelWork );
}

void WorkThread::GenerationStep( )
{
	wcout << __FUNCTION__ << endl;
    m_pPerformanceWindow->ComputationStart( );   // prepare for time measurement
    m_pEvolutionCore->Compute( m_pModelWork );   // compute next generation
    m_pPerformanceWindow->ComputationStop( );    // measure computation time
    m_pStatusBar->DisplayCurrentGeneration( );   // display new generation number in status bar
    ( * m_pDisplayGridFunctor )( FALSE );        // notify all views
}

void WorkThread::GenerationRun( )
{
    GenerationStep( );

    if ( m_bContinue )
    {
        m_pPerformanceWindow->SleepDelay( );
        postMsg2WorkThread( THREAD_MSG_GENERATION_RUN, 0, 0 );
    }
}

void WorkThread::processScript( wstring * const pwstr )
{
    ++m_iScriptLevel;
    if ( ! ProcessScript( * pwstr ) )
        (void)TerminateProcess( GetCurrentProcess(), 2 ); //TODO: find better solution
    --m_iScriptLevel;
    delete pwstr;
}

void WorkThread::ApplyEditorCommand( tEvoCmd const command, short const sParam )
{
    switch ( command )
    {
    case tEvoCmd::editSetXvalue:
        m_gpEdit.x = sParam;
        break;

    case tEvoCmd::editSetYvalue:
        m_gpEdit.y = sParam;
        m_pModelWork->ModelDoEdit( m_gpEdit );
        break;

    case tEvoCmd::editSetBrushShape:
        m_pModelWork->SetBrushShape( static_cast<tShape>( sParam ) );
        break;

    case tEvoCmd::editSetBrushSize:
        m_pModelWork->SetBrushSize( sParam );
        break;

    case tEvoCmd::editSetBrushIntensity:
        m_pModelWork->SetBrushIntensity( sParam );
        break;

    case tEvoCmd::editSetBrushMode:
        m_pModelWork->SetBrushStrategy( static_cast<tBrushMode>( sParam ) );
        break;

    default:
        assert( false );
    }
}

void WorkThread::StopComputation()
{
	m_bContinue = FALSE;
}

void WorkThread::DoEdit( GridPoint const gp )
{
	m_pModelWork->ModelDoEdit( gp );
}

void WorkThread::DoExit(HWND hwndApp)
{
	m_bContinue = FALSE;
	PostMessage(hwndApp, WM_DESTROY, 0, 0);
}

DWORD WorkThread::processWorkerMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam  )
{
    switch (uiMsg)
    {
        
    case THREAD_MSG_RESET_MODEL:
        ResetModel( );
        break;

    case THREAD_MSG_PROCESS_SCRIPT:
        processScript( (wstring *)lParam );
        break;

    case THREAD_MSG_FORWARD_STEP:
        GenerationStep( );
        return 0;

    case THREAD_MSG_GENERATION_RUN:
        assert( m_iScriptLevel == 0 );
        GenerationRun( );
        return 0;

    case THREAD_MSG_STOP:
		StopComputation();
        return 0;

    case THREAD_MSG_SET_BRUSH_INTENSITY:
        ApplyEditorCommand( tEvoCmd::editSetBrushIntensity, static_cast<short>( wParam ) );
        break;

    case THREAD_MSG_SET_BRUSH_SIZE:
        ApplyEditorCommand( tEvoCmd::editSetBrushSize, static_cast<short>( wParam ) );
        break;

    case THREAD_MSG_SET_BRUSH_SHAPE:
        ApplyEditorCommand( tEvoCmd::editSetBrushShape, static_cast<short>( wParam ) );
        break;

    case THREAD_MSG_SET_BRUSH_MODE:
        ApplyEditorCommand( tEvoCmd::editSetBrushMode, static_cast<short>( wParam ) );
        break;

    case THREAD_MSG_DO_EDIT:
        DoEdit( GridPoint( wParam, lParam ) );
        break;

    case THREAD_MSG_REFRESH:
        break;

    case THREAD_MSG_EXIT:
        m_bContinue = FALSE;
        PostMessage( (HWND)lParam, WM_DESTROY, 0, 0 );
        return 0;

    default:
        break;
    }

    m_pStatusBar->DisplayCurrentGeneration( );
    ( * m_pDisplayGridFunctor )( FALSE );
    return 0;
}

static DWORD WINAPI WorkerThread( _In_ LPVOID lpParameter )
{
    WorkThread * const pWT = static_cast<WorkThread *>( lpParameter );
    MSG                msg;
       
    (void)PeekMessage( &msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE );  // cause creation of message queue
    (void)SetEvent( pWT->m_hEventThreadStarter );				        // trigger waiting thread to continue

    do
    {
        BOOL const bRet = GetMessage( &msg, nullptr, 0, 0 );
        assert( bRet >= 0 );
        (void)pWT->processWorkerMessage( msg.message, msg.wParam, msg.lParam );
    } while ( msg.message != pWT->THREAD_MSG_EXIT );

    return 0;
}

void WorkThread::postMsg2WorkThread( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    if ( m_iScriptLevel > 0 )
    {
        (void)processWorkerMessage( uiMsg, wParam, lParam );
    }
    else
    {
        if ( m_pDisplayGridFunctor != nullptr )
            ( * m_pDisplayGridFunctor ).Continue( );  // trigger worker thread if waiting for an event
        BOOL const bRes = PostThreadMessage( m_dwThreadId, uiMsg, wParam, lParam );
        DWORD err = GetLastError( );
        assert( bRes );
    }
}

// procedural interface of worker thread

void WorkThread::PostReset( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
    postMsg2WorkThread( THREAD_MSG_RESET_MODEL, 0, 0 );
}

void WorkThread::PostRefresh( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
    postMsg2WorkThread( THREAD_MSG_REFRESH, 0, 0 );
}

void WorkThread::SetGenerationDelay( DWORD const dwNewDelay )  // in milliseconds
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << dwNewDelay << endl;
    m_pPerformanceWindow->SetGenerationDelay( dwNewDelay );
}

void WorkThread::PostDoEdit( GridPoint const & gp )
{
    if ( gp.IsInGrid() )
    {
        if ( m_bTrace )
            * m_pTraceStream << __func__ << L" " << gp << endl;
        //lint -e571  suspicious cast <WPARAM>
        postMsg2WorkThread( THREAD_MSG_DO_EDIT, static_cast<WPARAM>(gp.x), static_cast<LPARAM>(gp.y) );
        //lint +e571
    }
}

void WorkThread::PostSetBrushIntensity( INT const iValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iValue << endl;
    postMsg2WorkThread( THREAD_MSG_SET_BRUSH_INTENSITY, iValue, 0 );
}

void WorkThread::PostSetBrushSize( INT const iValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iValue << endl;
    postMsg2WorkThread( THREAD_MSG_SET_BRUSH_SIZE, iValue, 0 );
}

void WorkThread::PostSetBrushMode( tBrushMode const mode )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetBrushModeName( mode ) << endl;
    postMsg2WorkThread( THREAD_MSG_SET_BRUSH_MODE, static_cast<WPARAM>( mode ), 0 );
}

void WorkThread::PostSetBrushShape( tShape const shape )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetShapeName( shape ) << endl;
    postMsg2WorkThread( THREAD_MSG_SET_BRUSH_SHAPE, static_cast<WPARAM>( shape ), 0 );
}

void WorkThread::PostNextGeneration(  )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

    wcout << __FUNCTION__ << endl;
    postMsg2WorkThread( THREAD_MSG_FORWARD_STEP, 0, 0 );
}

void WorkThread::PostRunGenerations( )
{
	m_bContinue = TRUE;
	postMsg2WorkThread(THREAD_MSG_GENERATION_RUN, 0, 0);
}

void WorkThread::PostStopComputation( )
{
	postMsg2WorkThread(THREAD_MSG_STOP, 0, 0);
}

void WorkThread::PostProcessScript( wstring const & wstrPath )
{
//lint -esym( 429, pwstr )     not freed here, happens in worker thread
    wstring * const pwstr = new wstring( wstrPath );
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" \"" << wstrPath.c_str( )  << "\""<< endl;

    postMsg2WorkThread( THREAD_MSG_PROCESS_SCRIPT, 0, (LPARAM)pwstr );
}

// no trace output

void WorkThread::PostEndThread( HWND const hwndCtl )
{
    postMsg2WorkThread( THREAD_MSG_STOP, 0, 0 );
    postMsg2WorkThread( THREAD_MSG_EXIT, 0, (LPARAM)hwndCtl );
}

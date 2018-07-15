// win32_worker_thread.cpp
//

#include "stdafx.h"
#include "gridRect.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "EvolutionModelData.h"
#include "EvoHistorySys.h"
#include "EvolutionCore.h"
#include "win32_packGridPoint.h"
#include "win32_hiResTimer.h"
#include "win32_script.h"
#include "win32_status.h"
#include "win32_performanceWindow.h"
#include "win32_editor.h"
#include "win32_displayAll.h"
#include "win32_worker_thread.h"

using namespace std;

//lint -e849    same enumerator value          
//lint -esym( 488, tThreadMessages::THREAD_MSG_LAST )  same value as THREAD_MSG_EXIT

WorkThread::WorkThread( wostream * pTraceStream ) :
    m_hEventThreadStarter( nullptr ),
    m_dwThreadId         ( 0 ),
	m_hThread            ( nullptr ),
    m_bTrace             ( TRUE ),
    m_pTraceStream       ( pTraceStream ),
    m_hTimer             ( nullptr ),
    m_iScriptLevel       ( 0 ),
    m_pStatusBar         ( nullptr ),
    m_pPerformanceWindow ( nullptr ),
	m_pEditorWindow      ( nullptr ),
    m_pDisplayGridFunctor( nullptr ),
    m_pEvolutionCore     ( nullptr ),
    m_pModelWork         ( nullptr ),
    m_pEvoHistorySys     ( nullptr ),
    m_bContinue          ( FALSE ),
    m_genDemanded        ( 0 )
{ }

void WorkThread::Start
( 
    StatusBar          * const pStatus, 
    PerformanceWindow  * const pPerformanceWindow,
	EditorWindow       * const pEditorWindow,
    DisplayAll   const * const pDisplayGridFunctor,
    EvolutionCore      * const pEvolutionCore,
    EvolutionModelData * const pModel,
    EvoHistorySys      * const pEvoHistorySys

)
{
    m_hThread             = Util::MakeThread( WorkerThread, this, &m_dwThreadId, &m_hEventThreadStarter );
    m_pPerformanceWindow  = pPerformanceWindow;
	m_pEditorWindow       = pEditorWindow;
    m_pStatusBar          = pStatus;
    m_pDisplayGridFunctor = pDisplayGridFunctor;
    m_pEvolutionCore      = pEvolutionCore;
    m_pModelWork          = pModel;
	m_pEvoHistorySys      = pEvoHistorySys;


    (void)SetThreadAffinityMask( m_hThread, 0x0002 );

    m_pEvolutionCore->SetGridDisplayFunctor( m_pDisplayGridFunctor );   // display callback for core
}

WorkThread::~WorkThread( )
{
    m_hEventThreadStarter = nullptr;
    m_pTraceStream        = nullptr;
    m_hTimer              = nullptr;
	m_hThread             = nullptr;
    m_pStatusBar          = nullptr;
    m_pPerformanceWindow  = nullptr;
    m_pEditorWindow       = nullptr;
    m_pDisplayGridFunctor = nullptr;
	m_pEvoHistorySys      = nullptr;
}

// GenerationStep - perform one history step towards demanded generation
//                - update editor state if neccessary

void WorkThread::GenerationStep( )   // Layer 5
{
	if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )
	{
		if ( 
		       (m_pEvoHistorySys->GetCurrentGeneration( ) < m_genDemanded)  &&
		       (m_pEvoHistorySys->GetCurrentGeneration( ) == m_pEvoHistorySys->GetYoungestGeneration( ))
		   )
			m_pEvoHistorySys->EvoCreateNextGenCommand( );
		else
		{
			m_pEvoHistorySys->EvoApproachHistGen( m_genDemanded ); // Get a stored generation from history system
			if ( EditorStateHasChanged( ) )                        // editor state may be different from before
			{                                                       
				SaveEditorState( );
				if (m_pEditorWindow != nullptr)                    // make sure that editor GUI 
					m_pEditorWindow->UpdateEditControls( );        // reflects new state
			}
		}

		workMessage( THREAD_MSG_REFRESH, 0, 0 );
    
		if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )
			WorkPostGenerationStep(  );   // Loop! Will call indirectly HistWorkThread::GenerationStep again
	}
}

void WorkThread::ComputeNextGeneration( ) // Layer 1
{
	if (m_pPerformanceWindow != nullptr)
	    m_pPerformanceWindow->ComputationStart( );   // prepare for time measurement
    m_pEvolutionCore->Compute( m_pModelWork );       // compute next generation
	if (m_pPerformanceWindow != nullptr)
		m_pPerformanceWindow->ComputationStop( );    // measure computation time

	if (m_pStatusBar != nullptr)
	    m_pStatusBar->DisplayCurrentGeneration( m_pModelWork->GetEvoGenerationNr( ) );   // display new generation number in status bar
	if (m_pDisplayGridFunctor != nullptr)
	    ( * m_pDisplayGridFunctor )( FALSE );        // notify all views
}

void WorkThread::postGotoGeneration( HIST_GENERATION const gen )
{
    assert( gen >= 0 );

	m_genDemanded = gen;
    workMessage( THREAD_MSG_STEP, 0, 0 );    // will call indirectly HistWorkThread::GenerationStep
}

void WorkThread::generationRun( )  // Layer 1
{
    GenerationStep( );

    if ( m_bContinue )
    {
		if (m_pPerformanceWindow != nullptr)
		    m_pPerformanceWindow->SleepDelay( );
        PostRunGenerations( );
    }
}

void WorkThread::DoProcessScript( wstring * const pwstr )
{
    ++m_iScriptLevel;
    Script::ProcessScript( * pwstr );
    --m_iScriptLevel;
    delete pwstr;
}

void WorkThread::stopComputation()
{
	m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( );
	m_bContinue = FALSE;
	Script::StopProcessing( );
}

void WorkThread::workMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    if ( m_iScriptLevel > 0 )
    {
        dispatchMessage( uiMsg, wParam, lParam );
    }
    else
    {
        postMessage( uiMsg, wParam, lParam );
    }
}

void WorkThread::postMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    if ( m_pDisplayGridFunctor != nullptr )
        ( * m_pDisplayGridFunctor ).Continue( );  // trigger worker thread if waiting for an event
    BOOL const bRes = PostThreadMessage( m_dwThreadId, uiMsg, wParam, lParam );
    DWORD err = GetLastError( );
    assert( bRes );
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
        (void)pWT->dispatchMessage( msg.message, msg.wParam, msg.lParam );
    } while ( msg.message != pWT->THREAD_MSG_EXIT );

    return 0;
}

void WorkThread::dispatchMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam  )
{
    switch ( uiMsg )   // Layer 6
    {
        
    case THREAD_MSG_PROCESS_SCRIPT:
        DoProcessScript( (wstring *)lParam );
        break;

    case THREAD_MSG_STEP:
        GenerationStep( );  // call HistWorkThread::GenerationStep( ) 
        break;

    case THREAD_MSG_GENERATION_RUN:
        assert( m_iScriptLevel == 0 );
		m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( ) + 1 ;
		generationRun( );
        break;

    case THREAD_MSG_STOP:
		stopComputation();
        break;

    case THREAD_MSG_RESET_MODEL:
		m_pEvolutionCore->ResetModel( m_pModelWork );
        break;

    case THREAD_MSG_SET_BRUSH_INTENSITY:
    case THREAD_MSG_SET_BRUSH_SIZE:
    case THREAD_MSG_SET_BRUSH_SHAPE:
    case THREAD_MSG_SET_BRUSH_MODE:
    case THREAD_MSG_DO_EDIT:
        editorCommand( uiMsg, wParam );
        break;

    case THREAD_MSG_REFRESH:
        break;

    default:
        break;
    }

	if (m_pStatusBar != nullptr)
	    m_pStatusBar->DisplayCurrentGeneration( m_pModelWork->GetEvoGenerationNr( ) );   // display new generation number in status bar
	if (m_pDisplayGridFunctor != nullptr)
	    ( * m_pDisplayGridFunctor )( FALSE );
}

void WorkThread::editorCommand( UINT const uiMsg, WPARAM const wParam )
{
		static unordered_map < UINT, tEvoCmd > mapTable =
		{
			{ THREAD_MSG_SET_BRUSH_INTENSITY, tEvoCmd::editSetBrushIntensity },
			{ THREAD_MSG_SET_BRUSH_SIZE,      tEvoCmd::editSetBrushSize      },
			{ THREAD_MSG_SET_BRUSH_SHAPE,     tEvoCmd::editSetBrushShape     },
			{ THREAD_MSG_SET_BRUSH_MODE,      tEvoCmd::editSetBrushMode      },
			{ THREAD_MSG_DO_EDIT,             tEvoCmd::editDoEdit            }
		};
    
    if ( m_pEvoHistorySys->EvoCreateEditorCommand( mapTable.at( uiMsg ), static_cast<unsigned short>( wParam ) ) )
        SaveEditorState( );
	if ( uiMsg != THREAD_MSG_DO_EDIT )
		m_pEditorWindow->UpdateEditControls( );
}

BOOL WorkThread::EditorStateHasChanged( ) 
{ 
	return m_pEvolutionCore->EditorStateHasChanged( m_pModelWork ); 
}

void WorkThread::SaveEditorState( ) 
{ 
	return m_pEvolutionCore->SaveEditorState( m_pModelWork ); 
}

// procedural interface of worker thread (Layer 7)

void WorkThread::PostReset( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
    workMessage( THREAD_MSG_RESET_MODEL, 0, 0 );
}

void WorkThread::PostRefresh( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
    workMessage( THREAD_MSG_REFRESH, 0, 0 );
}

void WorkThread::PostDoEdit( GridPoint const & gp )
{
    if ( gp.IsInGrid() )
    {
        if ( m_bTrace )
            * m_pTraceStream << __func__ << L" " << gp << endl;
        workMessage( THREAD_MSG_DO_EDIT, gp.Pack2short(), 0 );
    }
}

void WorkThread::PostSetBrushIntensity( INT const iValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iValue << endl;
    workMessage( THREAD_MSG_SET_BRUSH_INTENSITY, iValue, 0 );
}

void WorkThread::PostSetBrushSize( INT const iValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iValue << endl;
    workMessage( THREAD_MSG_SET_BRUSH_SIZE, iValue, 0 );
}

void WorkThread::PostSetBrushMode( tBrushMode const mode )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetBrushModeName( mode ) << endl;
    workMessage( THREAD_MSG_SET_BRUSH_MODE, static_cast<WPARAM>( mode ), 0 );
}

void WorkThread::PostSetBrushShape( tShape const shape )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetShapeName( shape ) << endl;
    workMessage( THREAD_MSG_SET_BRUSH_SHAPE, static_cast<WPARAM>( shape ), 0 );
}

void WorkThread::WorkPostGenerationStep(  )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

    workMessage( THREAD_MSG_STEP, 0, 0 );
}

void WorkThread::PostRunGenerations( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	m_bContinue = TRUE;
	workMessage( THREAD_MSG_GENERATION_RUN, 0, 0 );
}

void WorkThread::ResetModel( )    // Layer 5
{
	m_pEvoHistorySys->EvoCreateResetCommand( );
}

void WorkThread::PostRedo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistorySys->GetCurrentGeneration( );

	if ( ( gen < m_pEvoHistorySys->GetYoungestGeneration( ) ) && m_pEvoHistorySys->IsEditorCommand( gen + 1 ) )
		postGotoGeneration( gen + 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThread::PostGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	postGotoGeneration( m_pEvoHistorySys->GetCurrentGeneration( ) + 1 );
}

void WorkThread::PostUndo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistorySys->GetCurrentGeneration( );

	if ( ( gen > 0 ) && m_pEvoHistorySys->IsEditorCommand( gen - 1 ) )
		postGotoGeneration( gen - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThread::PostPrevGeneration( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	if (m_pEvoHistorySys->GetCurrentGeneration() > 0)
		postGotoGeneration( m_pEvoHistorySys->GetCurrentGeneration() - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThread::PostHistoryAction( UINT const uiID, GridPoint const gp )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << uiID << L" " << gp << endl;

	assert( m_pModelWork->IsAlive(gp) );
	assert( (uiID == IDM_GOTO_ORIGIN) || (uiID == IDM_GOTO_DEATH) );

	IndId           idTarget  = m_pModelWork->GetId(gp);
	HIST_GENERATION genTarget = ( uiID == IDM_GOTO_ORIGIN )
	                            ? m_pEvoHistorySys->GetFirstGenOfIndividual(idTarget)
		                        : m_pEvoHistorySys->GetLastGenOfIndividual(idTarget);
	
	postGotoGeneration( genTarget );
}

void WorkThread::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << gen << endl;

	postGotoGeneration( gen );
}
void WorkThread::PostStopComputation( )
{
	workMessage( THREAD_MSG_STOP, 0, 0 );
}

void WorkThread::PostProcessScript( wstring const & wstrPath )
{
//lint -esym( 429, pwstr )     not freed here, happens in worker thread
    wstring * const pwstr = new wstring( wstrPath );
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" \"" << wstrPath.c_str( )  << "\""<< endl;

    workMessage( THREAD_MSG_PROCESS_SCRIPT, 0, (LPARAM)pwstr );
}

// no trace output

void WorkThread::PostEndThread( HWND const hwndCtl )
{
	stopComputation();                                   // stop running computation and script processing
	postMessage( THREAD_MSG_EXIT, 0, (LPARAM)hwndCtl );  // stop message pump of thread
	::WaitForSingleObject(m_hThread, INFINITE);          // wait until thread has stopped
	DestroyWindow( hwndCtl);                             // trigger termination of application
}

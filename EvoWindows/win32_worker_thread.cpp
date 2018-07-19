// win32_worker_thread.cpp
//

#include "stdafx.h"
#include "gridRect.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "EvolutionModelData.h"
#include "EvoHistorySys.h"
#include "EvolutionCore.h"
#include "win32_script.h"
#include "win32_performanceWindow.h"
#include "win32_editor.h"
#include "win32_displayAll.h"
#include "win32_workThreadInterface.h"
#include "win32_worker_thread.h"

using namespace std;

//lint -e849    same enumerator value          
//lint -esym( 488, tThreadMessages::THREAD_MSG_LAST )  same value as THREAD_MSG_EXIT

WorkThread::WorkThread( ) :
	m_pWorkThreadInterface( nullptr ),
    m_hEventThreadStarter ( nullptr ),
    m_dwThreadId          ( 0 ),
	m_hThread             ( nullptr ),
    m_iScriptLevel        ( 0 ),
    m_pPerformanceWindow  ( nullptr ),
	m_pEditorWindow       ( nullptr ),
    m_pDisplayGridFunctor ( nullptr ),
    m_pEvolutionCore      ( nullptr ),
    m_pModelWork          ( nullptr ),
    m_pEvoHistorySys      ( nullptr ),
    m_bContinue           ( FALSE ),
    m_genDemanded         ( 0 )
{ }

void WorkThread::Start
(  
    PerformanceWindow   * const pPerformanceWindow,
	EditorWindow        * const pEditorWindow,
    DisplayAll    const * const pDisplayGridFunctor,
    EvolutionCore       * const pEvolutionCore,
    EvolutionModelData  * const pModel,
    EvoHistorySys       * const pEvoHistorySys,
	WorkThreadInterface * const pWorkThreadInterface

)
{
    m_hThread              = Util::MakeThread( WorkerThread, this, &m_dwThreadId, &m_hEventThreadStarter );
    m_pPerformanceWindow   = pPerformanceWindow;
	m_pEditorWindow        = pEditorWindow;
    m_pDisplayGridFunctor  = pDisplayGridFunctor;
    m_pEvolutionCore       = pEvolutionCore;
    m_pModelWork           = pModel;
	m_pEvoHistorySys       = pEvoHistorySys;
	m_pWorkThreadInterface = pWorkThreadInterface;

    (void)SetThreadAffinityMask( m_hThread, 0x0002 );

    m_pEvolutionCore->SetGridDisplayFunctor( m_pDisplayGridFunctor );   // display callback for core
}

WorkThread::~WorkThread( )
{
    m_pWorkThreadInterface = nullptr;
	m_hEventThreadStarter  = nullptr;
	m_hThread              = nullptr;
    m_pPerformanceWindow   = nullptr;
    m_pEditorWindow        = nullptr;
    m_pDisplayGridFunctor  = nullptr;
	m_pEvoHistorySys       = nullptr;
}

void WorkThread::TerminateThread( HWND const hwndCtl )
{
	dispatchMessage( THREAD_MSG_STOP, 0, 0 );            // stop running computation and script processing
	postMessage( THREAD_MSG_EXIT, 0, (LPARAM)hwndCtl );  // stop message pump of thread
	::WaitForSingleObject(m_hThread, INFINITE);          // wait until thread has stopped
	DestroyWindow( hwndCtl);                             // trigger termination of application
}

// GenerationStep - perform one history step towards demanded generation
//                - update editor state if neccessary

void WorkThread::GenerationStep( )   
{
	if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )  // is something to do at all?
	{
		if ( 
		       (m_pEvoHistorySys->GetCurrentGeneration( ) < m_genDemanded)  &&
		       (m_pEvoHistorySys->GetCurrentGeneration( ) == m_pEvoHistorySys->GetYoungestGeneration( ))
		   )     
		{                                                    // Normal case: Compute next generation
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->ComputationStart( );   // prepare for time measurement
			m_pEvoHistorySys->EvoCreateNextGenCommand( );
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->ComputationStop( );    // measure computation time
		}
		else  // we are somewhere in history
		{
			m_pEvoHistorySys->EvoApproachHistGen( m_genDemanded ); // Get a stored generation from history system
			if ( m_pModelWork->EditorStateHasChanged( ) )          // editor state may be different from before
			{                                                       
				m_pModelWork->SaveEditorState( );
				if (m_pEditorWindow != nullptr)                    // make sure that editor GUI 
					m_pEditorWindow->UpdateEditControls( );        // reflects new state
			}
		}

		WorkMessage( THREAD_MSG_REFRESH, 0, 0 );                   // refresh all views
    
		if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )   // still not done?
			m_pWorkThreadInterface->PostRepeatGenerationStep( );   // Loop! Will call indirectly GenerationStep again
	}
}

void WorkThread::generationRun( )
{
    if ( m_bContinue )
    {
		assert( m_iScriptLevel == 0 );
		m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( ) + 1 ;
    
		GenerationStep( );

		if (m_pPerformanceWindow != nullptr)
		    m_pPerformanceWindow->SleepDelay( );
        m_pWorkThreadInterface->PostRunGenerations( false );
    }
}

void WorkThread::DoProcessScript( wstring * const pwstr )
{
    ++m_iScriptLevel;
    Script::ProcessScript( * pwstr );
    --m_iScriptLevel;
    delete pwstr;
}

// WorkMessage - process incoming messages from main thread

void WorkThread::WorkMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    if ( m_iScriptLevel > 0 )                      // if we are processing a script    
    {                                              // we run already in worker thread 
        dispatchMessage( uiMsg, wParam, lParam );  // dispatch message directly to avoid blocking
    }
    else                                           // normal case
    {                                              // we run in main thread
        postMessage( uiMsg, wParam, lParam );      // post message to worker thread
    }
}

void WorkThread::postMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    if ( m_pDisplayGridFunctor != nullptr )
        ( * m_pDisplayGridFunctor ).Continue( );  // trigger worker thread if waiting for an event
	assert( IsValidThreadMessage( uiMsg ) );
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

    for(;;)
    {
        BOOL const bRet = GetMessage( &msg, nullptr, 0, 0 );
        assert( bRet >= 0 );
		if ( msg.message == pWT->THREAD_MSG_EXIT )
			break;
        (void)pWT->dispatchMessage( msg.message, msg.wParam, msg.lParam );
    } 

    return 0;
}

void WorkThread::dispatchMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam  )
{
    switch ( uiMsg )   // Layer 6
    {
        
    case THREAD_MSG_PROCESS_SCRIPT:
        DoProcessScript( (wstring *)lParam );
        break;

    case THREAD_MSG_REPEAT_GENERATION_STEP:
        GenerationStep( ); 
        break;

    case THREAD_MSG_GOTO_GENERATION:
		m_genDemanded = HIST_GENERATION(static_cast<long>(lParam));
        GenerationStep( ); 
        break;

    case THREAD_MSG_GENERATION_RUN:
		if ( static_cast<bool>(lParam) )
			m_bContinue = TRUE;
		generationRun( );
        break;

    case THREAD_MSG_STOP:
		m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( );
		m_bContinue = FALSE;
		Script::StopProcessing( );
        break;

    case THREAD_MSG_RESET_MODEL:
    case THREAD_MSG_SET_BRUSH_INTENSITY:
    case THREAD_MSG_SET_BRUSH_SIZE:
    case THREAD_MSG_SET_BRUSH_SHAPE:
    case THREAD_MSG_SET_BRUSH_MODE:
    case THREAD_MSG_DO_EDIT:
    case THREAD_MSG_SET_POI:
        editorCommand( uiMsg, wParam );
        break;

    case THREAD_MSG_REFRESH:
        break;

    default:
		return;  // sometimes strange messages arrive. e.g. uiMsg 1847
    }            // I cannot find a reason, so I ignore them

	if (m_pDisplayGridFunctor != nullptr)
	    ( * m_pDisplayGridFunctor )( FALSE );
}

void WorkThread::editorCommand( UINT const uiMsg, WPARAM const wParam )
{
	static unordered_map < UINT, tEvoCmd > mapTable =
	{
		{ THREAD_MSG_RESET_MODEL,         tEvoCmd::reset                 },
		{ THREAD_MSG_SET_BRUSH_INTENSITY, tEvoCmd::editSetBrushIntensity },
		{ THREAD_MSG_SET_BRUSH_SIZE,      tEvoCmd::editSetBrushSize      },
		{ THREAD_MSG_SET_BRUSH_SHAPE,     tEvoCmd::editSetBrushShape     },
		{ THREAD_MSG_SET_BRUSH_MODE,      tEvoCmd::editSetBrushMode      },
		{ THREAD_MSG_DO_EDIT,             tEvoCmd::editDoEdit            },
		{ THREAD_MSG_SET_POI,             tEvoCmd::editSetPOI            }
	};
    
	tEvoCmd cmd = mapTable.at( uiMsg );
    if ( m_pEvoHistorySys->EvoCreateEditorCommand( cmd, static_cast<unsigned short>( wParam ) ) )
	{
        m_pModelWork->SaveEditorState(  );
	}

	if ( (m_pEditorWindow != nullptr) && IsEditorCommand( cmd ) )
	{
		 m_pEditorWindow->UpdateEditControls( );
	}
}

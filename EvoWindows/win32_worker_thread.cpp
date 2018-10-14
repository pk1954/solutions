// win32_worker_thread.cpp
//

#include "stdafx.h"
#include "gridRect.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "GridPoint24.h"
#include "EvolutionCore.h"
#include "EvoHistorySysGlue.h"
#include "EventInterface.h"
#include "ObserverInterface.h"
#include "win32_script.h"
#include "win32_editor.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_performanceWindow.h"
#include "win32_workThreadInterface.h"
#include "win32_worker_thread.h"

using namespace std;

WorkThread::WorkThread( ) :
	m_pWorkThreadInterface( nullptr ),
    m_iScriptLevel        ( 0 ),
    m_pPerformanceWindow  ( nullptr ),
	m_pEditorWindow       ( nullptr ),
    m_pObservers          ( nullptr ),
    m_pEvent              ( nullptr ),
    m_pCore               ( nullptr ),
    m_pEvoHistGlue        ( nullptr ),
    m_bContinue           ( FALSE ),
    m_genDemanded         ( 0 )
{ }

void WorkThread::Start
(  
    PerformanceWindow   * const pPerformanceWindow,
	EditorWindow        * const pEditorWindow,
    EventInterface      * const pEvent,
    ObserverInterface   * const pObservers, 
    EvolutionCore       * const pCore,
    EvoHistorySysGlue   * const pEvoHistorySys,
	WorkThreadInterface * const pWorkThreadInterface
)
{
    m_pPerformanceWindow   = pPerformanceWindow;
	m_pEditorWindow        = pEditorWindow;
    m_pEvent               = pEvent;
	m_pObservers           = pObservers;
    m_pCore                = pCore;
	m_pEvoHistGlue         = pEvoHistorySys;
	m_pWorkThreadInterface = pWorkThreadInterface;
    StartThread( TRUE, L"Worker" );  // start thread as a loop
    SetThreadAffinityMask( 0x0002 );
}

void WorkThread::ThreadStartupFunc()
{
	Continue( );   // trigger mother thread to continue
}

WorkThread::~WorkThread( )
{
    m_pWorkThreadInterface = nullptr;
    m_pPerformanceWindow   = nullptr;
    m_pEditorWindow        = nullptr;
    m_pEvent               = nullptr;
    m_pObservers           = nullptr;
	m_pEvoHistGlue         = nullptr;
}

void WorkThread::TerminateThread( HWND const hwndCtl )
{
	ThreadMsgDispatcher( MSG { nullptr, THREAD_MSG_STOP, 0, 0 } );  // stop running computation and script processing
	Terminate( );
	DestroyWindow( hwndCtl );                       // trigger termination of application
}

// GenerationStep - perform one history step towards demanded generation
//                - update editor state if neccessary

void WorkThread::GenerationStep( )   
{
	if ( m_pEvoHistGlue->GetCurrentGeneration( ) != m_genDemanded )  // is something to do at all?
	{
		if ( 
		       (m_pEvoHistGlue->GetCurrentGeneration( ) < m_genDemanded)  &&
		       (m_pEvoHistGlue->GetCurrentGeneration( ) == m_pEvoHistGlue->GetYoungestGeneration( ))
		   )     
		{                                                    // Normal case: Compute next generation
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->ComputationStart( );   // prepare for time measurement
			m_pEvoHistGlue->EvoCreateNextGenCommand( );
			if (m_pPerformanceWindow != nullptr)
				m_pPerformanceWindow->ComputationStop( );    // measure computation time
		}
		else  // we are somewhere in history
		{
			m_pEvoHistGlue->EvoApproachHistGen( m_genDemanded ); // Get a stored generation from history system
			if (m_pEditorWindow != nullptr)              
				m_pEditorWindow->UpdateEditControls( );          // make sure that editor GUI reflects new state
		}

		WorkMessage( THREAD_MSG_REFRESH, 0, 0 );                   // refresh all views
    
		if ( m_pEvoHistGlue->GetCurrentGeneration( ) != m_genDemanded )   // still not done?
			m_pWorkThreadInterface->PostRepeatGenerationStep( );   // Loop! Will call indirectly GenerationStep again
	}
}

void WorkThread::generationRun( )
{
    if ( m_bContinue )
    {
		assert( m_iScriptLevel == 0 );
		m_genDemanded = m_pEvoHistGlue->GetCurrentGeneration( ) + 1 ;
    
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

void WorkThread::WorkMessage( UINT const uiMsg, WPARAM const wparam, LPARAM const lparam )
{
	WorkMessage( MSG { nullptr, uiMsg, wparam, lparam } );
}

void WorkThread::WorkMessage( MSG const msg )
{
	assert( IsValidThreadMessage( msg.message ) );
    if ( m_iScriptLevel > 0 )                      // if we are processing a script    
    {                                              // we run already in worker thread 
        ThreadMsgDispatcher( msg );                // dispatch message directly to avoid blocking
    }
    else                                           // normal case
    {                                              // we run in main thread
		if ( m_pEvent != nullptr )
			m_pEvent->Continue( );                 // trigger worker thread if waiting for an event
		PostThreadMessage( msg.message, msg.wParam, msg.lParam );  // post message to worker thread
	}
}

void WorkThread::ThreadMsgDispatcher( MSG const msg  )
{
    switch ( msg.message )
    {
        
    case THREAD_MSG_PROCESS_SCRIPT:
        DoProcessScript( (wstring *)msg.lParam );
        break;

    case THREAD_MSG_REPEAT_GENERATION_STEP:
        GenerationStep( ); 
        break;

    case THREAD_MSG_GOTO_GENERATION:
		m_genDemanded = HIST_GENERATION(static_cast<long>(msg.lParam));
        GenerationStep( ); 
        break;

    case THREAD_MSG_GENERATION_RUN:
		if ( static_cast<bool>(msg.lParam) )
			m_bContinue = TRUE;
		generationRun( );
        break;

    case THREAD_MSG_STOP:
		m_genDemanded = m_pEvoHistGlue->GetCurrentGeneration( );
		m_bContinue = FALSE;
		Script::StopProcessing( );
        break;

    case THREAD_MSG_RESET_MODEL:
        editorCommand( tEvoCmd::reset, msg.wParam );
		if ( static_cast<BOOL>(msg.wParam) )
			m_pEvoHistGlue->ClearHistory( );
        break;

    case THREAD_MSG_SET_BRUSH_MODE:
        editorCommand( tEvoCmd::editSetBrushMode, msg.wParam );
        break;

    case THREAD_MSG_SET_BRUSH_RADIUS:
        editorCommand( tEvoCmd::editSetBrushRadius, msg.wParam );
        break;

    case THREAD_MSG_SET_BRUSH_SHAPE:
        editorCommand( tEvoCmd::editSetBrushShape, msg.wParam );
        break;

    case THREAD_MSG_SET_BRUSH_OPERATOR:
        editorCommand( tEvoCmd::editSetBrushManipulator, msg.wParam );
        break;

    case THREAD_MSG_SET_BRUSH_INTENSITY:
        editorCommand( tEvoCmd::editSetBrushIntensity, msg.wParam );
        break;

    case THREAD_MSG_DO_EDIT:
        editorCommand( tEvoCmd::editDoEdit, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
        break;

    case THREAD_MSG_SET_POI:
        editorCommand( tEvoCmd::editSetPOI, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
        break;

    case THREAD_MSG_SET_SIMULATION_MODE:
        if ( editorCommand( tEvoCmd::setSimulationMode, msg.wParam ) )
			m_pEditorWindow->SetSimulationMode();
        break;

    case THREAD_MSG_REFRESH:
        break;

    default:
		return;  // sometimes strange messages arrive. e.g. uiMsg 1847
    }             // I cannot find a reason, so I ignore them

	if (m_pObservers != nullptr)
	    m_pObservers->Notify( );
}

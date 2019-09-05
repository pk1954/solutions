// win32_EvoWorkThread.cpp
//
// EvoWindows

#include "stdafx.h"
#include <sstream> 
#include "gridRect.h"
#include "SCRIPT.H"
#include "EvoHistorySysGlue.h"
#include "EventInterface.h"
#include "EvoReadBuffer.h"
#include "win32_delay.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_actionTimer.h"
#include "win32_colorManager.h"
#include "win32_util_resource.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_EvoWorkThread.h"

EvoWorkThread::EvoWorkThread
( 
	HWND                     const hwndApplication,
	ColorManager           * const pColorManager,
	ActionTimer            * const pActionTimer,
	EventInterface         * const pEvent,
	Delay                  * const pDelay,
	ObserverInterface      * const pObserver, 
	EvoHistorySysGlue      * const pEvoHistorySys,
	EvoWorkThreadInterface * const pWorkThreadInterface
) :
	m_pColorManager       ( pColorManager ),
	m_pActionTimer        ( pActionTimer ),
	m_pDelay              ( pDelay ),   
	m_pEventPOI           ( pEvent ),   
	m_pObserver           ( pObserver ),   
	m_pEvoHistGlue        ( pEvoHistorySys ),   
	m_pWorkThreadInterface( pWorkThreadInterface ),
	m_hwndApplication     ( hwndApplication ),
	m_bContinue           ( FALSE ),
	m_genDemanded         ( 0 )
{
	StartThread( L"WorkerThread", true ); 
}

EvoWorkThread::~EvoWorkThread( )
{
	m_hwndApplication      = nullptr;
	m_pColorManager        = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pActionTimer         = nullptr;
	m_pEventPOI            = nullptr;
	m_pObserver            = nullptr;
	m_pEvoHistGlue         = nullptr;
	m_pDelay               = nullptr;
}

bool EvoWorkThread::userWantsHistoryCut( ) const
{
	HIST_GENERATION genCurrent  = m_pEvoHistGlue->GetCurrentGeneration( );
	HIST_GENERATION genYoungest = m_pEvoHistGlue->GetYoungestGeneration( );
	std::wostringstream wBuffer;
	wBuffer << L"Gen " << ( genCurrent + 1 ) << L" - " << genYoungest << L" will be deleted.";
	int iRes = MessageBox( nullptr, L"Cut off history?", wBuffer.str( ).c_str( ), MB_OKCANCEL | MB_SYSTEMMODAL );
	return ( iRes == IDOK );
}

// WorkMessage - process incoming messages from main thread

void EvoWorkThread::WorkMessage
( 
	BOOL                    const isEditOperation,
	WorkerThreadMessage::Id const msg, 
	WPARAM                  const wparam, 
	LPARAM                  const lparam
)
{
	assert( WorkerThreadMessage::IsValid( msg ) );
	
	if ( isEditOperation &&  m_pEvoHistGlue->IsInHistoryMode() )
	{
		if ( userWantsHistoryCut( ) )
		{
			m_pEvoHistGlue->EvoClearHistory( m_pEvoHistGlue->GetCurrentGeneration( ) );  // cut off future generations
		}
		else 
		{
			return;            // user answered no, cancel operation
		}
	}

	PostThreadMsg( static_cast<UINT>(msg), wparam, lparam );
}

void EvoWorkThread::ThreadStartupFunc( ) 
{ 
	SetThreadAffinityMask( 0x0002 );
}

void EvoWorkThread::ThreadMsgDispatcher( MSG const msg  )
{
	try
	{
		dispatch( msg );
	}
	catch ( HistoryBufferException e )
	{
		PostMessage( m_hwndApplication, WM_COMMAND, IDM_HIST_BUFFER_FULL, 0 );
	}
}

void EvoWorkThread::dispatch( MSG const msg  )
{
	switch ( static_cast<WorkerThreadMessage::Id>(msg.message) )
	{

	case WorkerThreadMessage::Id::GENERATION_RUN:
		if ( static_cast<bool>(msg.lParam) )          // if first RUN message ...
			setContinueFlag( TRUE );
		generationRun( );
		break;

	case WorkerThreadMessage::Id::STOP:
		m_genDemanded = m_pEvoHistGlue->GetCurrentGeneration( );
		setContinueFlag( FALSE );
		Script::StopProcessing( );
		return;      // do not notify readbuffer, because model has not changed  

	case WorkerThreadMessage::Id::REPEAT_GENERATION_STEP:
		gotoGeneration( m_genDemanded );
		break;

	case WorkerThreadMessage::Id::GENERATION_STEP:
		gotoGeneration( m_pEvoHistGlue->GetCurrentGeneration( ) + 1 );
		break;

	case WorkerThreadMessage::Id::BENCHMARK:
		NGenerationSteps( static_cast<int>(msg.lParam) );
		break;

	case WorkerThreadMessage::Id::GOTO_GENERATION:
		gotoGeneration( HIST_GENERATION(static_cast<long>(msg.lParam)) );
		break;

	case WorkerThreadMessage::Id::GOTO_ORIGIN:
	case WorkerThreadMessage::Id::GOTO_DEATH:
		gotoGeneration
		( 
			m_pEvoHistGlue->GetGenWithIndividual
			(
				GridPoint
				( 
					static_cast<GRID_COORD>( CastToShort(msg.wParam) ), 
					static_cast<GRID_COORD>( CastToShort(msg.lParam) )
				),
				static_cast<WorkerThreadMessage::Id>(msg.message) == WorkerThreadMessage::Id::GOTO_DEATH 
			)
		);
		break;

	case WorkerThreadMessage::Id::PREV_GENERATION:
		{
			HIST_GENERATION gen = m_pEvoHistGlue->GetCurrentGeneration( );

			if ( gen > 0 )
				gotoGeneration( gen - 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkerThreadMessage::Id::REDO:
		{
			HIST_GENERATION gen = m_pEvoHistGlue->GetCurrentGeneration( );

			if ( ( gen < m_pEvoHistGlue->GetYoungestGeneration( ) ) && m_pEvoHistGlue->IsEditorCommand( gen + 1 ) )
				gotoGeneration( gen + 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkerThreadMessage::Id::UNDO:
		{
			HIST_GENERATION gen = m_pEvoHistGlue->GetCurrentGeneration( );

			if ( ( gen > 0 ) && m_pEvoHistGlue->IsEditorCommand( gen - 1 ) )
				gotoGeneration( gen - 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkerThreadMessage::Id::RESET_MODEL:
		editorCommand( tEvoCmd::reset, msg.wParam );
		if ( static_cast<BOOL>(msg.wParam) )
			m_pEvoHistGlue->EvoClearAllHistory( );
		break;

	case WorkerThreadMessage::Id::SET_BRUSH_MODE:
		editorCommand( tEvoCmd::editSetBrushMode, msg.wParam );
		break;

	case WorkerThreadMessage::Id::SET_BRUSH_RADIUS:
		editorCommand( tEvoCmd::editSetBrushRadius, msg.wParam );
		break;

	case WorkerThreadMessage::Id::SET_BRUSH_SHAPE:
		editorCommand( tEvoCmd::editSetBrushShape, msg.wParam );
		break;

	case WorkerThreadMessage::Id::SET_BRUSH_OPERATOR:
		editorCommand( tEvoCmd::editSetBrushManipulator, msg.wParam );
		break;

	case WorkerThreadMessage::Id::SET_BRUSH_INTENSITY:
		editorCommand( tEvoCmd::editSetBrushIntensity, msg.wParam );
		break;

	case WorkerThreadMessage::Id::DO_EDIT:
		editorCommand( tEvoCmd::editDoEdit, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
		break;

	case WorkerThreadMessage::Id::SET_POI:
		editorCommand( tEvoCmd::editSetPOI, GridPoint24( CastToUnsignedInt(msg.wParam), CastToUnsignedInt(msg.lParam) ) );
		break;

	case WorkerThreadMessage::Id::SET_STRATEGY_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::individual, static_cast<Strategy::Id>(msg.wParam) );
		break;

	case WorkerThreadMessage::Id::SET_SELECTION_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::selection );
		break;

	case WorkerThreadMessage::Id::SET_HIGHLIGHT_COLOR:
		m_pColorManager->SetColor( static_cast<COLORREF>(msg.lParam), tColorObject::highlight );
		break;

	case WorkerThreadMessage::Id::REFRESH:
		break;

	default:
		return;  // sometimes strange messages arrive. e.g. uiMsg 1847
	}            // I cannot find a reason, so I ignore them

	if (m_pObserver != nullptr)                // notify main thread, that model has changed.
		m_pObserver->Notify( ! m_bContinue );  // continue immediately, if in run mode and
}                                              // main thread is busy.

// gotoGeneration - perform one history step towards demanded generation
//                - update editor state if neccessary

void EvoWorkThread::gotoGeneration( HIST_GENERATION const gen )   
{
	m_genDemanded = gen;
	if ( m_pEvoHistGlue->GetCurrentGeneration( ) != m_genDemanded )  // is something to do at all?
	{
		if ( 
				(m_pEvoHistGlue->GetCurrentGeneration( ) < m_genDemanded)  &&
				(m_pEvoHistGlue->GetCurrentGeneration( ) == m_pEvoHistGlue->GetYoungestGeneration( ))
			)     
		{                                                    // Normal case: Compute next generation
			if (m_pActionTimer != nullptr)
				m_pActionTimer->TimerStart( );               // prepare for time measurement

			m_pEvoHistGlue->EvoCreateNextGenCommand( );      //////// here the real work is done! ////////////////

			if (m_pActionTimer != nullptr)
				m_pActionTimer->TimerStop( );                     // measure computation time
		}
		else  // we are somewhere in history
		{
			m_pEvoHistGlue->EvoApproachHistGen( m_genDemanded );   // Get a stored generation from history system
		}

		WorkMessage( FALSE, WorkerThreadMessage::Id::REFRESH, 0, 0 );     // refresh all views
    
		if ( m_pEvoHistGlue->GetCurrentGeneration( ) != m_genDemanded )   // still not done?
			m_pWorkThreadInterface->PostRepeatGenerationStep( );          // Loop! Will call indirectly gotoGeneration again
	}
}

void EvoWorkThread::NGenerationSteps( int iNrOfGenerations )  // for benchmarks only
{
	Stopwatch stopwatch;
	stopwatch.Start();
	for (int i = 0; i < iNrOfGenerations; ++i)
	{
		m_pEvoHistGlue->EvoCreateNextGenCommand( );
		WorkMessage( FALSE, WorkerThreadMessage::Id::REFRESH, 0, 0 );   // refresh all views
	}
	stopwatch.Stop( L"benchmark" );
}

void EvoWorkThread::generationRun( )
{
	if ( m_bContinue )
	{
		gotoGeneration( m_pEvoHistGlue->GetCurrentGeneration( ) + 1 );

		if (m_pDelay != nullptr)
			m_pDelay->SleepDelay( );

		m_pWorkThreadInterface->PostRunGenerations( false );
	}
}

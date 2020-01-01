// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "SCRIPT.H"
#include "EventInterface.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_stopwatch.h"
#include "win32_actionTimer.h"
#include "win32_util_resource.h"
#include "win32_WorkThreadInterface.h"
#include "win32_WorkThread.h"

WorkThread::WorkThread
( 
	HWND                  const hwndApplication,
	ActionTimer         * const pActionTimer,
	EventInterface      * const pEvent,
	ObserverInterface   * const pObserver,
	ModelInterface      * const pModel,
	WorkThreadInterface * const pWorkThreadInterface,
	BOOL                  const bAsync
) :
	m_pComputeTimer       ( pActionTimer ),
	m_pEventPOI           ( pEvent ),   
	m_pObserver           ( pObserver ),   
	m_pModel              ( pModel ),
	m_pWorkThreadInterface( pWorkThreadInterface ),
	m_hwndApplication     ( hwndApplication ),
	m_bContinue           ( FALSE )
{
	StartThread( L"WorkerThread", bAsync ); 
}

WorkThread::~WorkThread( )
{
	m_hwndApplication      = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pComputeTimer        = nullptr;
	m_pEventPOI            = nullptr;
	m_pObserver            = nullptr;
}

// WorkMessage - process incoming messages from main thread

void WorkThread::WorkMessage
( 
	BOOL                  const isEditOperation,
	WorkThreadMessage::Id const msg, 
	WPARAM                const wparam, 
	LPARAM                const lparam
)
{
	PostThreadMsg( static_cast<UINT>(msg), wparam, lparam );
}

void WorkThread::ThreadStartupFunc( ) 
{ 
	SetThreadAffinityMask( 0x0002 );
}

void WorkThread::ThreadMsgDispatcher( MSG const msg  )
{
	if ( 
			WorkThread::Dispatch( msg )  // Give WorkThread a chance to handle message by itself. 
			|| Dispatch( msg )           // Otherwise hand over to application
		)                               // If one of these could handle the message, ...
	{
		if (m_pObserver != nullptr)              // ... notify main thread, that model has changed.
			m_pObserver->Notify( m_bContinue );  // Continue immediately, if in run mode and
	}
	else  // Nobody could handle message
	{
		// Sometimes strange messages arrive. e.g. uiMsg 1847
	}   // I cannot find a reason, so I ignore them
}

BOOL WorkThread::Dispatch( MSG const msg )
{
	switch ( static_cast<WorkThreadMessage::Id>(msg.message) )
	{

	case WorkThreadMessage::Id::GENERATION_RUN:
		generationRun( static_cast<bool>(msg.lParam) );
		break;

	case WorkThreadMessage::Id::STOP:
		generationStop( );
		return FALSE;      // do not notify observers, because model has not changed  

	case WorkThreadMessage::Id::NEXT_GENERATION:
		Compute( );  // compute next generation
		break;


	case WorkThreadMessage::Id::REFRESH:
		break;

	default:
		return FALSE; // could not handle message
	} 

	return TRUE; // message handled, all done
}

void WorkThread::generationRun( bool const bFirst )
{
	if ( bFirst )               // if first RUN message ...
		setRunMode( TRUE );

	if ( m_bContinue )
	{
		Compute( );  // compute next generation

		WaitTilNextActivation( );

		if ( m_pWorkThreadInterface->IsAsyncThread() )
		{
			m_pWorkThreadInterface->PostRunGenerations( false );
		}
		else
		{
			PostMessage( m_hwndApplication, WM_COMMAND, IDM_RUN, 0 );
		}
	}
}

void WorkThread::generationStop( )
{
	setRunMode( FALSE );
	Script::StopProcessing( );
}

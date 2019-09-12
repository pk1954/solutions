// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "SCRIPT.H"
#include "EventInterface.h"
#include "NNetHistorySysGlue.h"
#include "NNetReadBuffer.h"
#include "win32_delay.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_actionTimer.h"
#include "win32_util_resource.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWorkThread.h"

NNetWorkThread::NNetWorkThread
( 
	HWND                      const hwndApplication,
	ActionTimer             * const pActionTimer,
	EventInterface          * const pEvent,
	Delay                   * const pDelay,
	ObserverInterface       * const pObserver, 
	NNetHistorySysGlue      * const pNNetHistorySys,
	NNetWorkThreadInterface * const pWorkThreadInterface
):
	WorkThread
	( 
		hwndApplication, 
		pActionTimer, 
		pEvent, 
		pDelay, 
		pObserver, 
		pNNetHistorySys->GetHistorySystem( ), 
		pWorkThreadInterface 
	)
{
	StartThread( L"NNetWorkThread", true ); 
}

NNetWorkThread::~NNetWorkThread( )
{
}

BOOL NNetWorkThread::Dispatch( MSG const msg  )
{
	switch ( static_cast<NNetWorkThreadMessage::Id>(msg.message) )
	{

	case NNetWorkThreadMessage::Id::REDO:
		{
		HIST_GENERATION genCurrent  = GetHistorySystem( )->GetCurrentGeneration( );
		HIST_GENERATION genYoungest = GetHistorySystem( )->GetYoungestGeneration( );


			if ( ( genCurrent < genYoungest ) && isEditorCommand( genCurrent + 1 ) )
				GotoGeneration( genCurrent + 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case NNetWorkThreadMessage::Id::UNDO:
		{
		HIST_GENERATION genCurrent = GetHistorySystem( )->GetCurrentGeneration( );

			if ( ( genCurrent > 0 ) && isEditorCommand( genCurrent - 1 ) )
				GotoGeneration( genCurrent - 1 );
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}

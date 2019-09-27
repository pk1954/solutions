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
		pObserver, 
		pNNetHistorySys->GetHistorySystem( ), 
		pWorkThreadInterface 
	),
	m_pDelay( pDelay )
{
}

NNetWorkThread::~NNetWorkThread( )
{
}

BOOL NNetWorkThread::Dispatch( MSG const msg  )
{
	switch ( static_cast<NNetWorkThreadMessage::Id>(msg.message) )
	{


	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::WaitTilNextActivation( )
{
	static microseconds  const TIME_RESOLUTION    =  microseconds( 100 );
	static unsigned long const SLOW_MOTION_FACTOR = 100;

	//if (m_pDelay != nullptr)
	//	m_pDelay->SleepDelay( );

	m_hrTimer.BusyWait( TIME_RESOLUTION * SLOW_MOTION_FACTOR );
}

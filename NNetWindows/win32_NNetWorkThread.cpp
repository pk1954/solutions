// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "SCRIPT.H"
#include "SlowMotionRatio.h"
#include "EventInterface.h"
#include "NNetHistorySysGlue.h"
#include "NNetReadBuffer.h"
#include "NNetTypes.h"
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
	ObserverInterface       * const pObserver, 
	SlowMotionRatio         * const pSLowMotionRatio,
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
	m_pSlowMotionRatio( pSLowMotionRatio ),
	m_timerTicks( Ticks( 0 ) )
{
}

NNetWorkThread::~NNetWorkThread( )
{
}

BOOL NNetWorkThread::Dispatch( MSG const msg  )
{
	switch ( static_cast<NNetWorkThreadMessage::Id>(msg.message) )
	{
	case NNetWorkThreadMessage::Id::NNET_FIRST:
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::WaitTilNextActivation( )
{
	//if (m_pDelay != nullptr)
	//	m_pDelay->SleepDelay( );

	m_hrTimer.BusyWait( TIME_RESOLUTION * m_pSlowMotionRatio->GetRatio(), m_timerTicks );
}

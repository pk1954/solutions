// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "SCRIPT.H"
#include "SlowMotionRatio.h"
#include "EventInterface.h"
#include "NNetReadBuffer.h"
#include "NNetModel.h"
#include "MoreTypes.h"
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
	NNetWorkThreadInterface * const pWorkThreadInterface,
	NNetModel               * const pNNetModel
):
	m_pNNetModel( pNNetModel ),
	WorkThread
	( 
		hwndApplication, 
		pActionTimer, 
		pEvent, 
		pObserver,
		nullptr,    // no history system
		pNNetModel,
		pWorkThreadInterface 
	),
	m_pSlowMotionRatio( pSLowMotionRatio ),
	m_timerTicksLastTime( Ticks( 0 ) ),
	m_timerTicks( Ticks( 0 ) ),
	m_dutyCycle( 0.0 )
{
}

NNetWorkThread::~NNetWorkThread( )
{
}

BOOL NNetWorkThread::Dispatch( MSG const msg  )
{
	switch ( static_cast<NNetWorkThreadMessage::Id>(msg.message) )
	{
	case NNetWorkThreadMessage::Id::HIGHLIGHT:
		m_pNNetModel->HighlightShape( ShapeId( CastToInt(msg.lParam) ) );
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::WaitTilNextActivation( )
{
	Ticks workTicks = m_hrTimer.ReadHiResTimer() - m_timerTicksLastTime;

	m_hrTimer.BusyWait( TIME_RESOLUTION * m_pSlowMotionRatio->GetRatio(), m_timerTicks );

	Ticks ticksAct   = m_hrTimer.ReadHiResTimer();
	Ticks cycleTicks = ticksAct - m_timerTicksLastTime;

	m_dutyCycle = static_cast<double>( workTicks.GetValue() ) / static_cast<double>( cycleTicks.GetValue() ); 

	m_timerTicksLastTime = ticksAct;
} 

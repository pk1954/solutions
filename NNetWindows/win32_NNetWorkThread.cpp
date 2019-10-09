// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "SCRIPT.H"
#include "PixelTypes.h"
#include "SlowMotionRatio.h"
#include "EventInterface.h"
#include "NNetParameters.h"
#include "NNetReadBuffer.h"
#include "NNetModel.h"
#include "MoreTypes.h"
#include "win32_util.h"
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
	case NNetWorkThreadMessage::Id::TRIGGER:
	{
		ShapeId const id  ( CastToUnsignedLong( msg.wParam ) );
		Shape       * shape( m_pNNetModel->GetShape( id ) );
		InputNeuron * pInputNeuron( Cast2InputNeuron( shape ) );
		pInputNeuron->Trigger();
	}
	break;

	case NNetWorkThreadMessage::Id::HIGHLIGHT:
		m_pNNetModel->HighlightShape( ShapeId( CastToUnsignedLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_FREQ:
	{
		ShapeId const id  ( CastToUnsignedLong( msg.wParam ) );
		Shape       * shape( m_pNNetModel->GetShape( id ) );
		InputNeuron * pInputNeuron( Cast2InputNeuron( shape ) );
		Hertz   const freq( CastToUnsignedLong( msg.lParam ) );
		pInputNeuron->SetPulseFrequency( freq );
	}
	break;

	case NNetWorkThreadMessage::Id::PULSE_SPEED:
	{
		ShapeId     const id   ( CastToUnsignedLong( msg.wParam ) );
		Shape           * shape( m_pNNetModel->GetShape( id ) );
		Pipeline        * pPipe( Cast2Pipeline( shape ) );
		meterPerSec const speed( (float&) msg.lParam );
		pPipe->SetPulseSpeed( speed );
	}
	break;

	case NNetWorkThreadMessage::Id::MOVE_SHAPE_TO:
	{
		ShapeId         const id( CastToUnsignedLong( msg.wParam ) );
		Shape               * pShape( m_pNNetModel->GetShape( id ) );
		MicroMeterPoint const newPos( Util::Unpack2MicroMeterPoint(msg.lParam) );
		pShape->MoveTo( newPos );
	}
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

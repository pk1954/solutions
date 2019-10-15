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
	SlowMotionRatio         * const pSlowMotionRatio,
	NNetWorkThreadInterface * const pWorkThreadInterface,
	NNetModel               * const pNNetModel,
	BOOL                      const bAsync
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
		pWorkThreadInterface,
		bAsync
	),
	m_pSlowMotionRatio( pSlowMotionRatio ),
	m_dutyCycle( 0.0 )
{
	m_hrTimer.Start();
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
		pPipe->SetPulseSpeed( * m_pNNetModel, speed );
	}
	break;

	case NNetWorkThreadMessage::Id::MOVE_SHAPE_TO:
	{
		ShapeId         const id( CastToUnsignedLong( msg.wParam ) );
		Shape               * pShape( m_pNNetModel->GetShape( id ) );
		MicroMeterPoint const newPos( Util::Unpack2MicroMeterPoint(msg.lParam) );
		pShape->MoveTo( * m_pNNetModel, newPos );
	}
	break;

	case NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED:
		ResetTimer();
		m_pNNetModel->ResetSimulationTime();
		break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_BRANCH:
	{
		ShapeId const id { CastToUnsignedLong( msg.wParam ) };
		m_pNNetModel->CreateNewBranch( id );
	}
	break;

	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::WaitTilNextActivation( )
{
	Sleep( 10 );
} 

void NNetWorkThread::Compute() 
{ 
	Ticks        const ticksTilStart      = m_hrTimer.GetTicksTilStart( );                   
	microseconds const usTilStartRealTime = m_hrTimer.TicksToMicroseconds( ticksTilStart ); 
	microseconds const usTilStartSimuTime = usTilStartRealTime / m_pSlowMotionRatio->GetRatio();
	microseconds const usActualSimuTime   = m_pNNetModel->GetSimulationTime( );                // get actual time stamp
	microseconds const usMissingSimuTime  = usTilStartSimuTime - usActualSimuTime;             // compute missing simulation time
	if ( usMissingSimuTime > 0ms )
	{
		unsigned long ulCyclesTodo = CastToUnsignedLong( usMissingSimuTime / TIME_RESOLUTION ); // compute cycles to be computed
		do
			m_pNNetModel->Compute(); 
		while ( ulCyclesTodo-- );
	}
}
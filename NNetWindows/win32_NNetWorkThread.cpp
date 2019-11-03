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
			//ShapeId const id( CastToLong( msg.wParam ) );
			//Shape       * shape( m_pNNetModel->GetShape( id ) );
			//InputNeuron * pInputNeuron( Cast2InputNeuron( shape ) );
			//pInputNeuron->Trigger();
		}
		break;

	case NNetWorkThreadMessage::Id::CONNECT:
		m_pNNetModel->Connect( * m_pNNetModel );
		break;

	case NNetWorkThreadMessage::Id::HIGHLIGHT:
		m_pNNetModel->HighlightShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::SUPER_HIGHLIGHT:
		m_pNNetModel->SuperHighlightShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_FREQ:
		{
			InputNeuron * pInputNeuron( m_pNNetModel->GetInputNeuron( ShapeId( CastToLong(msg.wParam) ) ) );
			pInputNeuron->SetPulseFrequency( fHertz( (float &)msg.lParam ) );
		}
		break;

	case NNetWorkThreadMessage::Id::DAMPING_FACTOR:
		m_pNNetModel->SetDampingFactor( (float &)msg.lParam );
		break;

	case NNetWorkThreadMessage::Id::THRESHHOLD_POTENTIAL:
		m_pNNetModel->SetThresholdPotential( mV( (float &)msg.lParam ) );
		break;

	case NNetWorkThreadMessage::Id::PEAK_VOLTAGE:
		m_pNNetModel->SetPeakVoltage( mV( (float &)msg.lParam ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_WIDTH:       
		m_pNNetModel->SetPulseWidth( microseconds( msg.lParam ) );
		break;

	case NNetWorkThreadMessage::Id::REFRACTORY_PERIOD:
		m_pNNetModel->SetRefractoryPeriod( microseconds( msg.lParam ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_SPEED:
		m_pNNetModel->SetImpulseSpeed( meterPerSec( (float &)msg.lParam ) );
		break;

	case NNetWorkThreadMessage::Id::MOVE_SHAPE_TO:
		{
			Shape               * pShape( m_pNNetModel->GetShape( ShapeId( CastToLong(msg.wParam) ) ) );
			MicroMeterPoint const newPos( Util::Unpack2MicroMeterPoint(msg.lParam) );
			pShape->MoveTo( * m_pNNetModel, newPos );
		}
		break;

	case NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED:
		ResetTimer();
		m_pNNetModel->ResetSimulationTime();
		break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_BRANCH:
		m_pNNetModel->CreateNewBranch( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_NEURON:
	{
		MicroMeterPoint const pnt 
		{ 
			static_cast<MicroMeter>((float &)msg.wParam), 
			static_cast<MicroMeter>((float &)msg.lParam) 
		};
		m_pNNetModel->CreateNewNeuron( pnt );
	}
	break;

	case NNetWorkThreadMessage::Id::SPLIT_PIPELINE:
	{
		ShapeId         const id( CastToLong( msg.wParam ) );
		MicroMeterPoint const pos( Util::Unpack2MicroMeterPoint(msg.lParam) );
		m_pNNetModel->SplitPipeline( id, pos );
	}
	break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_INPUT_NEURON:
	{
		MicroMeterPoint const pnt 
		{ 
			static_cast<MicroMeter>((float &)msg.wParam), 
			static_cast<MicroMeter>((float &)msg.lParam) 
		};
		m_pNNetModel->CreateNewInputNeuron( pnt );
	}
	break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_OUTPUT_NEURON:
	{
		MicroMeterPoint const pnt 
		{ 
			static_cast<MicroMeter>((float &)msg.wParam), 
			static_cast<MicroMeter>((float &)msg.lParam) 
		};
		m_pNNetModel->CreateNewOutputNeuron( pnt );
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
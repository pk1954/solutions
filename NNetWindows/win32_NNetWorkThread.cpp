// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include <unordered_map>
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

using std::unordered_map;

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
}

NNetWorkThread::~NNetWorkThread( )
{
}

MicroMeterPoint const NNetWorkThread::msg2MicroMeterPoint( MSG const msg ) const
{
	MicroMeterPoint const pnt 
	{ 
		static_cast<MicroMeter>((float &)msg.wParam), 
		static_cast<MicroMeter>((float &)msg.lParam) 
	};
	return pnt;
}

static tParameter const GetParameterType( NNetWorkThreadMessage::Id const m )
{
	static unordered_map < NNetWorkThreadMessage::Id, tParameter const > mapParam =
	{
		{ NNetWorkThreadMessage::Id::PULSE_RATE,        tParameter::pulseRate        },
		{ NNetWorkThreadMessage::Id::PULSE_SPEED,       tParameter::pulseSpeed       },
		{ NNetWorkThreadMessage::Id::PULSE_WIDTH,       tParameter::pulseWidth       },
		{ NNetWorkThreadMessage::Id::DAMPING_FACTOR,    tParameter::dampingFactor    },
		{ NNetWorkThreadMessage::Id::THRESHOLD,         tParameter::threshold        },
		{ NNetWorkThreadMessage::Id::PEAK_VOLTAGE,      tParameter::peakVoltage      },
		{ NNetWorkThreadMessage::Id::REFRACTORY_PERIOD, tParameter::refractoryPeriod }
	};				  

	return mapParam.at( m );
}

BOOL NNetWorkThread::Dispatch( MSG const msg  )
{
	NNetWorkThreadMessage::Id const id { static_cast<NNetWorkThreadMessage::Id>(msg.message) };
	switch ( id )
	{
	case NNetWorkThreadMessage::Id::TRIGGER:
		{
			//ShapeId const id( CastToLong( msg.wParam ) );
			//Shape       * shape( m_pNNetModel->GetShape( id ) );
			//InputNeuron * pInputNeuron( Cast2InputNeuron( shape ) );
			//pInputNeuron->Trigger();
		}
		break;

	case NNetWorkThreadMessage::Id::RESET_TIMER:
		m_pNNetModel->ResetSimulationTime();
		break;

	case NNetWorkThreadMessage::Id::CONNECT:
		m_pNNetModel->Connect( );
		break;

	case NNetWorkThreadMessage::Id::HIGHLIGHT:
		m_pNNetModel->HighlightShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::SUPER_HIGHLIGHT:
		m_pNNetModel->SuperHighlightShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_RATE:
	case NNetWorkThreadMessage::Id::DAMPING_FACTOR:
	case NNetWorkThreadMessage::Id::THRESHOLD:
	case NNetWorkThreadMessage::Id::PEAK_VOLTAGE:
	case NNetWorkThreadMessage::Id::PULSE_WIDTH:       
	case NNetWorkThreadMessage::Id::REFRACTORY_PERIOD:
	case NNetWorkThreadMessage::Id::PULSE_SPEED:
		m_pNNetModel->SetParameter
		( 
			GetParameterType( id ), 
			(float &)msg.lParam, 
			m_pNNetModel->GetShape( ShapeId( CastToLong(msg.wParam) ) )
		);
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
		m_pNNetModel->CreateNewNeuron( msg2MicroMeterPoint( msg ) );
		break;

	case NNetWorkThreadMessage::Id::SPLIT_PIPELINE:
	{
		ShapeId         const id( CastToLong( msg.wParam ) );
		MicroMeterPoint const pos( Util::Unpack2MicroMeterPoint(msg.lParam) );
		m_pNNetModel->SplitPipeline( id, pos );
	}
	break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_INPUT_NEURON:
		m_pNNetModel->CreateNewInputNeuron( msg2MicroMeterPoint( msg ) );
		break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_OUTPUT_NEURON:
		m_pNNetModel->CreateNewOutputNeuron( msg2MicroMeterPoint( msg ) );
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
	Ticks     const ticksTilStart      = m_hrTimer.GetTicksTilStart( );                   
	MicroSecs const usTilStartRealTime = m_hrTimer.TicksToMicroSecs( ticksTilStart ); 
	MicroSecs const usTilStartSimuTime = usTilStartRealTime / CastToFloat( m_pSlowMotionRatio->GetRatio() );
	MicroSecs const usActualSimuTime   = m_pNNetModel->GetSimulationTime( );                // get actual time stamp
	MicroSecs const usMissingSimuTime  = usTilStartSimuTime - usActualSimuTime;             // compute missing simulation time
	if ( usMissingSimuTime > 0._MicroSecs )
	{
		unsigned long ulCyclesTodo = CastToUnsignedLong( usMissingSimuTime / TIME_RESOLUTION ); // compute # cycles to be computed
		do
			m_pNNetModel->Compute();
		while ( ulCyclesTodo-- );
	}
}
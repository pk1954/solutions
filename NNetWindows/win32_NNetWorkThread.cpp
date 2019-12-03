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
	case NNetWorkThreadMessage::Id::RESET_TIMER:
		m_pNNetModel->ResetSimulationTime( );
		break;

	case NNetWorkThreadMessage::Id::CONNECT:
		m_pNNetModel->Connect( );
		break;

	case NNetWorkThreadMessage::Id::DELETE_SHAPE:
		m_pNNetModel->DeleteShape( );
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
		m_pNNetModel->SetParameter( GetParameterType( id ), (float &)msg.lParam	);
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

	case NNetWorkThreadMessage::Id::CREATE_NEW_NEURON:
		m_pNNetModel->CreateNewNeuron( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::SPLIT_PIPELINE:
		m_pNNetModel->SplitPipeline(  Util::Unpack2MicroMeterPoint(msg.lParam) );
	break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_INPUT_NEURON:
		m_pNNetModel->CreateNewInputNeuron( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::CREATE_NEW_OUTPUT_NEURON:
		m_pNNetModel->CreateNewOutputNeuron( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::WaitTilNextActivation( )
{
//	Sleep( 10 );
} 

void NNetWorkThread::Compute() 
{
	static DWORD nrOfCalls  = 0;
	static DWORD nrOfSleeps = 0;
	static DWORD nrOfCycles = 0;
	++nrOfCalls;
	Ticks     const ticksTilStart      = m_hrTimer.GetTicksTilStart( );                   
	MicroSecs const usTilStartRealTime = m_hrTimer.TicksToMicroSecs( ticksTilStart ); 
	MicroSecs const usTilStartSimuTime = usTilStartRealTime / CastToFloat( m_pSlowMotionRatio->GetRatio() );
	MicroSecs const usActualSimuTime   = m_pNNetModel->GetSimulationTime( );                // get actual time stamp
	MicroSecs const usMissingSimuTime  = usTilStartSimuTime - usActualSimuTime;             // compute missing simulation time
	if ( usMissingSimuTime > 0._MicroSecs )
	{
		unsigned long ulCyclesTodo = CastToUnsignedLong( usMissingSimuTime / TIME_RESOLUTION ); // compute # cycles to be computed
		nrOfCycles += ulCyclesTodo;
		do
			m_pNNetModel->Compute();
		while ( ulCyclesTodo-- );
	}
	else
	{
		Sleep( 10 );
		++nrOfSleeps;
	}
	double averageCyclesTodo = double(nrOfCycles) / double(nrOfCalls);
	int x = 55;
}
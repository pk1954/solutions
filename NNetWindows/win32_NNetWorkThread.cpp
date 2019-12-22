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
	m_pSlowMotionRatio( pSlowMotionRatio )
{
}

NNetWorkThread::~NNetWorkThread( )
{
}

static tParameter const GetParameterType( NNetWorkThreadMessage::Id const m )
{
	static unordered_map < NNetWorkThreadMessage::Id, tParameter const > mapParam =
	{
		{ NNetWorkThreadMessage::Id::PULSE_SPEED,       tParameter::pulseSpeed       },
		{ NNetWorkThreadMessage::Id::PULSE_WIDTH,       tParameter::pulseWidth       },
		{ NNetWorkThreadMessage::Id::DAMPING_FACTOR,    tParameter::signalLoss       },
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
		m_pNNetModel->Connect( ShapeId( CastToLong(msg.wParam) ), ShapeId( CastToLong(msg.lParam) ) );
		break;

	case NNetWorkThreadMessage::Id::REMOVE_SHAPE:
		m_pNNetModel->RemoveShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::PULSE_RATE:
		m_pNNetModel->SetPulseRate( ShapeId( CastToLong(msg.wParam) ), (float &)msg.lParam );
		break;

	case NNetWorkThreadMessage::Id::DAMPING_FACTOR:
	case NNetWorkThreadMessage::Id::THRESHOLD:
	case NNetWorkThreadMessage::Id::PEAK_VOLTAGE:
	case NNetWorkThreadMessage::Id::PULSE_WIDTH:       
	case NNetWorkThreadMessage::Id::REFRACTORY_PERIOD:
	case NNetWorkThreadMessage::Id::PULSE_SPEED:
		m_pNNetModel->SetParameter( GetParameterType( id ), (float &)msg.lParam	);
		break;

	case NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED:
		ResetTimer();
		m_pNNetModel->ResetSimulationTime();
		break;

	case NNetWorkThreadMessage::Id::MOVE_SHAPE:
		m_pNNetModel->MoveShape( ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::INSERT_NEURON:
		m_pNNetModel->InsertNeuron( ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::ADD_OUTGOING:
		m_pNNetModel->AddOutgoing( ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::ADD_INCOMING:
		m_pNNetModel->AddIncoming( ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::SPLIT_PIPELINE:
		m_pNNetModel->SplitPipeline( ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::ADD_NEURON:
		m_pNNetModel->AddNeuron( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::ADD_INPUT_NEURON:
		m_pNNetModel->AddInputNeuron( Util::Unpack2MicroMeterPoint(msg.lParam) );
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
	else
	{
		Sleep( 10 );
	}
}
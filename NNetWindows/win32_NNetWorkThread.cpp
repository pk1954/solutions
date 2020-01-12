// win32_worker_thread.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include <unordered_map>
#include "SCRIPT.H"
#include "Resource.h"
#include "PixelTypes.h"
#include "EventInterface.h"
#include "NNetParameters.h"
#include "NNetReadBuffer.h"
#include "NNetModel.h"
#include "MoreTypes.h"
#include "win32_util.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_actionTimer.h"
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
	m_pNNetModel             ( pNNetModel ),
	m_pEventPOI              ( pEvent ),   
	m_pModelObserver         ( pObserver ),   
	m_pWorkThreadInterface   ( pWorkThreadInterface ),
	m_hwndApplication        ( hwndApplication ),
	m_bContinue              ( FALSE ),
	m_pSlowMotionRatio       ( pSlowMotionRatio ),
	m_pTimeResObserver       ( nullptr ),
	m_usRealTimeSpentPerCycle( 0.0_MicroSecs )
{
	m_pTimeResObserver = new TimeResObserver( this );
	m_pNNetModel->AddParameterObserver( m_pTimeResObserver );   // notify me if parameters change
	m_pSlowMotionRatio->RegisterObserver( m_pTimeResObserver ); // notify ne if slomo ratio changes
	m_pTimeResObserver->Notify( TRUE );
	StartThread( L"WorkerThread", bAsync ); 
}

NNetWorkThread::~NNetWorkThread( )
{
	delete m_pTimeResObserver;
	m_pTimeResObserver     = nullptr;
	m_hwndApplication      = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pEventPOI            = nullptr;
	m_pModelObserver       = nullptr;
}

static tParameter const GetParameterType( NNetWorkThreadMessage::Id const m )
{
	static unordered_map < NNetWorkThreadMessage::Id, tParameter const > mapParam =
	{
		{ NNetWorkThreadMessage::Id::PULSE_SPEED,       tParameter::pulseSpeed     },
		{ NNetWorkThreadMessage::Id::PULSE_WIDTH,       tParameter::pulseWidth     },
		{ NNetWorkThreadMessage::Id::DAMPING_FACTOR,    tParameter::signalLoss     },
		{ NNetWorkThreadMessage::Id::THRESHOLD,         tParameter::threshold      },
		{ NNetWorkThreadMessage::Id::PEAK_VOLTAGE,      tParameter::peakVoltage    },
    	{ NNetWorkThreadMessage::Id::REFRACTORY_PERIOD, tParameter::refractPeriod  },
	    { NNetWorkThreadMessage::Id::TIME_RESOLUTION,   tParameter::timeResolution }
	};				  

	return mapParam.at( m );
}

void NNetWorkThread::ThreadStartupFunc( ) 
{ 
	SetThreadAffinityMask( 0x0002 );
}

void NNetWorkThread::ThreadMsgDispatcher( MSG const msg  )
{
	if ( dispatch( msg ) )
	{
		if (m_pModelObserver != nullptr)              // ... notify main thread, that model has changed.
			m_pModelObserver->Notify( m_bContinue );  // Continue immediately, if in run mode
	}
	else  // Nobody could handle message
	{
		// Sometimes strange messages arrive. e.g. uiMsg 1847
	}   // I cannot find a reason, so I ignore them
}

BOOL NNetWorkThread::dispatch( MSG const msg  )
{
	NNetWorkThreadMessage::Id const id { static_cast<NNetWorkThreadMessage::Id>(msg.message) };
	switch ( id )
	{
	case NNetWorkThreadMessage::Id::GENERATION_RUN:
		generationRun( static_cast<bool>(msg.lParam) );
		return TRUE;

	case NNetWorkThreadMessage::Id::STOP:
		generationStop( );
		return FALSE;

	case NNetWorkThreadMessage::Id::NEXT_GENERATION:
		compute( );  // compute next generation
		return TRUE;

	default:
		break;
	} 

	BOOL bResult { TRUE };

	m_pNNetModel->EnterCritSect();

	switch ( id )
	{
	case NNetWorkThreadMessage::Id::GENERATION_RUN:
		generationRun( static_cast<bool>(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::STOP:
		generationStop( );
		bResult = FALSE;
		break;

	case NNetWorkThreadMessage::Id::NEXT_GENERATION:
		compute( );  // compute next generation
		break;

	case NNetWorkThreadMessage::Id::REFRESH:
		break;

	case NNetWorkThreadMessage::Id::RESET_TIMER:
		m_pNNetModel->ResetSimulationTime( );
		break;

	case NNetWorkThreadMessage::Id::CONNECT:
		m_pNNetModel->Connect( ShapeId( CastToLong(msg.wParam) ), ShapeId( CastToLong(msg.lParam) ) );
		break;

	case NNetWorkThreadMessage::Id::REMOVE_SHAPE:
		m_pNNetModel->RemoveShape( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::DISCONNECT:
		m_pNNetModel->Disconnect( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::RESET_MODEL:
		m_pNNetModel->ResetModel( );
		m_pNNetModel->CreateInitialShapes();
		break;

	case NNetWorkThreadMessage::Id::PULSE_RATE:
		m_pNNetModel->SetPulseRate( ShapeId( CastToLong(msg.wParam) ), (float &)msg.lParam );
		break;

	case NNetWorkThreadMessage::Id::DAMPING_FACTOR:
	case NNetWorkThreadMessage::Id::THRESHOLD:
	case NNetWorkThreadMessage::Id::PEAK_VOLTAGE:
	case NNetWorkThreadMessage::Id::PULSE_WIDTH:       
	case NNetWorkThreadMessage::Id::REFRACTORY_PERIOD:
	case NNetWorkThreadMessage::Id::TIME_RESOLUTION:
	case NNetWorkThreadMessage::Id::PULSE_SPEED:
		m_pNNetModel->SetParameter( GetParameterType( id ), (float &)msg.lParam	);
		break;

	case NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED:
		m_hrTimer.Restart();
		m_pNNetModel->ResetSimulationTime();
		break;

	case NNetWorkThreadMessage::Id::MOVE_SHAPE:
	case NNetWorkThreadMessage::Id::INSERT_NEURON:
	case NNetWorkThreadMessage::Id::ADD_OUTGOING2KNOT:
	case NNetWorkThreadMessage::Id::ADD_INCOMING2KNOT:
	case NNetWorkThreadMessage::Id::ADD_OUTGOING2PIPE:
	case NNetWorkThreadMessage::Id::ADD_INCOMING2PIPE:
		actionCommand( id, ShapeId( CastToLong(msg.wParam) ), Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::NEW_NEURON:
		m_pNNetModel->NewShape<Neuron>( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::NEW_INPUT_NEURON:
		m_pNNetModel->NewShape<InputNeuron>( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::APPEND_NEURON:
		m_pNNetModel->AppendShape<Neuron>( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::APPEND_INPUT_NEURON:
		m_pNNetModel->AppendShape<InputNeuron>( ShapeId( CastToLong(msg.wParam) ) );
		break;

	default:
		bResult = FALSE;
		break;
	} 

	m_pNNetModel->LeaveCritSect();
	return bResult;
}

bool NNetWorkThread::actionCommand
( 
	NNetWorkThreadMessage::Id const   msgId,
	ShapeId                   const   id, 
	MicroMeterPoint           const & pos
)
{
	switch ( msgId )
	{
	case NNetWorkThreadMessage::Id::MOVE_SHAPE:
		m_pNNetModel->MoveShape( id, pos );
		break;

	case NNetWorkThreadMessage::Id::INSERT_NEURON:
		m_pNNetModel->InsertNeuron( id, pos );
		break;

	case NNetWorkThreadMessage::Id::ADD_OUTGOING2KNOT:
		m_pNNetModel->AddOutgoing2Knot( id, pos );
		break;

	case NNetWorkThreadMessage::Id::ADD_INCOMING2KNOT:
		m_pNNetModel->AddIncoming2Knot( id, pos );
		break;

	case NNetWorkThreadMessage::Id::ADD_OUTGOING2PIPE:
		m_pNNetModel->AddOutgoing2Pipe( id, pos );
		break;

	case NNetWorkThreadMessage::Id::ADD_INCOMING2PIPE:
		m_pNNetModel->AddIncoming2Pipe( id, pos );
		break;

	default:
		return FALSE;
	} 

	return TRUE;
}

void NNetWorkThread::generationRun( bool const bFirst )
{
	if ( bFirst )               // if first RUN message ...
	{
		m_bContinue = TRUE;
		m_runObservable.NotifyAll( TRUE);
		m_hrTimer.Start();
	}

	if ( m_bContinue )
	{
		compute( );  // compute next generation
		m_pWorkThreadInterface->PostRunGenerations( false );
	}
}

void NNetWorkThread::generationStop( )
{
	m_bContinue = FALSE;
	m_runObservable.NotifyAll( TRUE );
	m_hrTimer.Stop();
	Script::StopProcessing( );
}

void NNetWorkThread::TimeResObserver::Notify( bool const bImmediate )
{
	m_pNNetWorkThread->m_usRealTimeAvailPerCycle = 
		m_pNNetWorkThread->m_pSlowMotionRatio->SimuTime2RealTime
		( 
			m_pNNetWorkThread->m_pNNetModel->GetTimeResolution() 
		);
}

void NNetWorkThread::compute() 
{
	MicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
	MicroSecs const usTilStartSimuTime { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
	MicroSecs const usActualSimuTime   { m_pNNetModel->GetSimulationTime( ) };                                 // get actual time stamp
	MicroSecs const usMissingSimuTime  { usTilStartSimuTime - usActualSimuTime };                              // compute missing simulation time
	MicroSecs const usSimuTimeTodo     { min( usMissingSimuTime, m_pNNetModel->GetTimeResolution() ) };        // respect time slot (resolution)
	long      const lCyclesTodo        { CastToLong( usSimuTimeTodo / m_pNNetModel->GetTimeResolution( ) ) };  // compute # cycles to be computed
	for ( long lRun = 0; lRun < lCyclesTodo; ++lRun )
	{
		m_pNNetModel->Compute();
	}

	MicroSecs const usSpentInCompute { m_hrTimer.GetMicroSecsTilStart( ) - usTilStartRealTime };
	MicroSecs const usSleepTime      { m_usRealTimeAvailPerCycle - usSpentInCompute };
	if ( usSleepTime > 10000.0_MicroSecs )
		Sleep( 10 );
	if ( lCyclesTodo > 0 )
		m_usRealTimeSpentPerCycle = usSpentInCompute / CastToFloat(lCyclesTodo);
	m_performanceObservable.NotifyAll( FALSE);
}

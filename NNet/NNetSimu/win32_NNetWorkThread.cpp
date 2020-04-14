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
#include "MoreTypes.h"
#include "NNetModelStorage.h"
#include "SlowMotionRatio.h"
#include "NNetParameters.h"
#include "NNetModel.h"
#include "Analyzer.h"
#include "InputNeuron.h"
#include "win32_util.h"
#include "win32_thread.h"
#include "win32_event.h"
#include "win32_fatalError.h"
#include "win32_actionTimer.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWorkThread.h"

using std::unordered_map;

tAppCallBack NNetWorkThread::m_appCallback { nullptr };

void NNetWorkThread::InitClass( tAppCallBack AppCallBack )
{
	m_appCallback = AppCallBack;
}

NNetWorkThread::NNetWorkThread
( 
	HWND                      const hwndApplication,
	ActionTimer             * const pActionTimer,
	EventInterface          * const pEvent,
	ObserverInterface       * const pObserver, 
	SlowMotionRatio         * const pSlowMotionRatio,
	NNetWorkThreadInterface * const pWorkThreadInterface,
	NNetModel               * const pNNetModel,
	Param                   * const pParam,
	NNetModelStorage        * const pStorage,
	BOOL                      const bAsync
)
:	m_pStorage            ( pStorage ),
	m_pNNetModel          ( pNNetModel ),
	m_pParam              ( pParam ),
	m_pEventPOI           ( pEvent ),   
	m_pModelObserver      ( pObserver ),   
	m_pWorkThreadInterface( pWorkThreadInterface ),
	m_hwndApplication     ( hwndApplication ),
	m_pSlowMotionRatio    ( pSlowMotionRatio )
{
	m_pTimeResObserver = new TimeResObserver( this );
	m_pParam->AddParameterObserver( m_pTimeResObserver );       // notify me if parameters change
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
	m_pStorage             = nullptr;
}

static tParameter const GetParameterType( NNetWorkThreadMessage::Id const m )
{
	static unordered_map < NNetWorkThreadMessage::Id, tParameter const > mapParam =
	{
		{ NNetWorkThreadMessage::Id::PULSE_SPEED,       tParameter::pulseSpeed     },
		{ NNetWorkThreadMessage::Id::PULSE_WIDTH,       tParameter::pulseWidth     },
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
	BOOL bRes;
	try
	{
		bRes = dispatch( msg );
	}
	catch ( ... )
	{
		FatalError::Happened( 1000, "NNetWorkThread" );
	}

	if ( bRes )
	{
		if (m_pModelObserver != nullptr)                // ... notify main thread, that model has changed.
			m_pModelObserver->Notify( ! m_bContinue );  // Continue immediately, if in run mode
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

	switch ( id )
	{

	case NNetWorkThreadMessage::Id::SEND_BACK:
		(m_appCallback)( CastToInt(msg.wParam) );
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

	case NNetWorkThreadMessage::Id::CONVERT2NEURON:
		m_pNNetModel->Convert2Neuron( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::CONVERT2INPUT_NEURON:
		m_pNNetModel->Convert2InputNeuron( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::RESET_MODEL:
		generationStop( );
		m_pNNetModel->ResetModel( );
		m_pNNetModel->CreateInitialShapes();
		break;

	case NNetWorkThreadMessage::Id::PULSE_RATE:
		m_pNNetModel->SetPulseRate( ShapeId( CastToLong(msg.wParam) ), (float &)msg.lParam );
		m_pNNetModel->ClearModel( );
		break;

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

	case NNetWorkThreadMessage::Id::DELETE_SELECTION:
		m_pNNetModel->DeleteSelection();
		break;

	case NNetWorkThreadMessage::Id::COPY_SELECTION:
		m_pNNetModel->CopySelection();
		break;

	case NNetWorkThreadMessage::Id::ANALYZE_LOOPS:
	case NNetWorkThreadMessage::Id::ANALYZE_ANOMALIES:
		{
			generationStop( );
			m_pNNetModel->ClearModel( );
			m_pNNetModel->SelectAll( tBoolOp::opFalse );
			auto func { (id == NNetWorkThreadMessage::Id::ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
			bool bFound { func( * m_pNNetModel ) };
			if ( bFound )
				ModelAnalyzer::SelectLoopShapes( * m_pNNetModel );
			m_pParam->SetEmphasizeMode( bFound );
		}
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
		m_pParam->SetEmphasizeMode( false );
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
	m_pThread->m_usRealTimeAvailPerCycle = m_pThread->m_pSlowMotionRatio->SimuTime2RealTime( m_pThread->GetSimuTimeResolution() );
}

void NNetWorkThread::compute() 
{
	fMicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
	fMicroSecs const usTilStartSimuTime { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
	fMicroSecs const usActualSimuTime   { m_pNNetModel->GetSimulationTime( ) };                             // get actual time stamp
	fMicroSecs const usMissingSimuTime  { usTilStartSimuTime - usActualSimuTime };                          // compute missing simulation time
	fMicroSecs const usSimuTimeTodo     { min( usMissingSimuTime, m_pParam->GetTimeResolution() ) };        // respect time slot (resolution)
	long       const lCyclesTodo        { CastToLong( usSimuTimeTodo / m_pParam->GetTimeResolution( ) ) };  // compute # cycles to be computed
	for ( long lRun = 0; lRun < lCyclesTodo; ++lRun )
	{
		m_pNNetModel->Compute();
	}

	fMicroSecs const usSpentInCompute { m_hrTimer.GetMicroSecsTilStart( ) - usTilStartRealTime };
	fMicroSecs const usSleepTime      { m_usRealTimeAvailPerCycle - usSpentInCompute };
	if ( usSleepTime > 10000.0_MicroSecs )
		Sleep( 10 );
	if ( lCyclesTodo > 0 )
	{
		m_usRealTimeSpentPerCycle = usSpentInCompute / CastToFloat(lCyclesTodo);
		m_performanceObservable.NotifyAll( FALSE);
	}
}

fMicroSecs NNetWorkThread::GetSimuTimeResolution( ) const 
{ 
	return m_pParam->GetTimeResolution( ); 
}

fMicroSecs NNetWorkThread::GetSimulationTime( ) const 
{ 
	return m_pNNetModel->GetSimulationTime( ); 
}

float NNetWorkThread::GetSlowMotionRatio( ) const 
{ 
	return m_pSlowMotionRatio->GetRatio( ); 
}

// win32_WorkThread.cpp
//
// NNetSimu

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
#include "win32_WorkThreadInterface.h"
#include "win32_WorkThread.h"

using std::unordered_map;

void NNetWorkThread::InitClass( tAppCallBack AppCallBack )
{
	m_appCallback = AppCallBack;
}

NNetWorkThread::NNetWorkThread
( 
	HWND                  const hwndApplication,
	ActionTimer         * const pActionTimer,
	EventInterface      * const pEvent,
	SlowMotionRatio     * const pSlowMotionRatio,
	WorkThreadInterface * const pWorkThreadInterface,
	NNetModel           * const pNNetModel,
	Param               * const pParam,
	NNetModelStorage    * const pStorage,
	bool                  const bAsync
)
:	m_pStorage             ( pStorage ),
	m_pNNetModel           ( pNNetModel ),
	m_pParam               ( pParam ),
	m_pEventPOI            ( pEvent ),   
	m_pWorkThreadInterface ( pWorkThreadInterface ),
	m_hwndApplication      ( hwndApplication ),
	m_pSlowMotionRatio     ( pSlowMotionRatio )
{
	StartThread( L"NNetWorkThread", bAsync ); 
}

NNetWorkThread::~NNetWorkThread( )
{
	m_hwndApplication      = nullptr;
	m_pWorkThreadInterface = nullptr;
	m_pEventPOI            = nullptr;
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
	bool bRes;
	try
	{
		bRes = dispatch( msg );  // returns true, if model has changed
	}
	catch ( ... )
	{
		FatalError::Happened( 1000, "NNetWorkThread" );
	}

	if ( bRes ) 
	{
		// if ( m_pModelRedrawObservable )                // ... notify main thread, that model has changed.
		//	m_pModelRedrawObservable->NotifyAll( ! m_bContinue );  // Continue immediately, if in run mode
	}
	else  // Nobody could handle message
	{
		// Sometimes strange messages arrive. e.g. uiMsg 1847
	}   // I cannot find a reason, so I ignore them
}

bool NNetWorkThread::dispatch( MSG const msg ) // return true, if model has changed
{
	NNetWorkThreadMessage::Id const id { static_cast<NNetWorkThreadMessage::Id>(msg.message) };
	bool bResult { true };

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

	case NNetWorkThreadMessage::Id::TOGGLE_STOP_ON_TRIGGER:
		m_pNNetModel->ToggleStopOnTrigger( ShapeId( CastToLong(msg.wParam) ) );
		break;

	case NNetWorkThreadMessage::Id::SELECT_SHAPE:
		m_pNNetModel->SelectShape( ShapeId( CastToLong(msg.wParam) ), static_cast<tBoolOp>(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::SELECT_SUBTREE:
		m_pNNetModel->SelectSubtree( ShapeId( CastToLong(msg.wParam) ), static_cast<tBoolOp>(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::SELECT_ALL:
		m_pNNetModel->SelectAll( static_cast<tBoolOp>(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::RESET_MODEL:
		m_pNNetModel->ResetModel( );
		m_pNNetModel->CreateInitialShapes();
		break;

	case NNetWorkThreadMessage::Id::PULSE_RATE:
		m_pNNetModel->SetPulseRate_Lock( ShapeId( CastToLong(msg.wParam) ), static_cast<bool>(msg.lParam) ); 
		m_pNNetModel->ClearModel( );
		break;

	case NNetWorkThreadMessage::Id::TRIGGER_SOUND:
		m_pNNetModel->SetTriggerSound( ShapeId( CastToLong(msg.wParam) ), static_cast<Hertz>(Util::UnpackUlongA(msg.lParam)), static_cast<MilliSecs>(Util::UnpackUlongB(msg.lParam)) );
		m_pNNetModel->ClearModel( );
		break;

	case NNetWorkThreadMessage::Id::THRESHOLD:
	case NNetWorkThreadMessage::Id::PEAK_VOLTAGE:
	case NNetWorkThreadMessage::Id::PULSE_WIDTH:       
	case NNetWorkThreadMessage::Id::REFRACTORY_PERIOD:
	case NNetWorkThreadMessage::Id::TIME_RESOLUTION:
	case NNetWorkThreadMessage::Id::PULSE_SPEED:
		m_pNNetModel->SetParameter( GetParameterType( id ), (float &)msg.lParam	); //TODO: change to modern cast
		break;

	case NNetWorkThreadMessage::Id::MARK_SELECTION:
		m_pNNetModel->MarkSelection( static_cast<tBoolOp>(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::REMOVE_SELECTION:
		m_pNNetModel->DeleteSelection();
		break;

	case NNetWorkThreadMessage::Id::SELECT_ALL_BEEPERS:
		m_pNNetModel->SelectBeepers();
		break;

	case NNetWorkThreadMessage::Id::REMOVE_BEEPERS:
		m_pNNetModel->RemoveBeepers( );
		break;

	case NNetWorkThreadMessage::Id::COPY_SELECTION:
		m_pNNetModel->CopySelection();
		break;

	case NNetWorkThreadMessage::Id::ANALYZE_LOOPS:
	case NNetWorkThreadMessage::Id::ANALYZE_ANOMALIES:
		{
			m_pNNetModel->SelectAll( tBoolOp::opFalse );
			auto func { (id == NNetWorkThreadMessage::Id::ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
			bool bFound { func( * m_pNNetModel ) };
			if ( bFound )
				ModelAnalyzer::SelectLoopShapes( * m_pNNetModel );
			m_pParam->SetEmphasizeMode( bFound );
		}
		break;

	case NNetWorkThreadMessage::Id::MOVE_SELECTION:
		m_pNNetModel->MoveSelection( Util::Unpack2MicroMeterPoint(msg.lParam) );
		break;

	case NNetWorkThreadMessage::Id::SELECT_SHAPES_IN_RECT:
		m_pNNetModel->SelectShapesInRect( MicroMeterRect( Util::Unpack2MicroMeterPoint(msg.wParam), Util::Unpack2MicroMeterPoint(msg.lParam) ) );
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
		bResult = false;
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
		return false;
	} 

	return true;
}

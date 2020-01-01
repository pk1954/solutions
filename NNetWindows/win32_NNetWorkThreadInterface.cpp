// win32_NNetWorkThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "Resource.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_script.h"
#include "NNetReadBuffer.h"
#include "win32_NNetWorkThread.h"
#include "win32_NNetWorkThreadInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

NNetWorkThreadInterface::NNetWorkThreadInterface( ) :
	m_pTraceStream( nullptr ),
	m_bTrace      ( TRUE ),
	m_pNNetWorkThread( nullptr ),
	m_pModel( nullptr )
{ }

NNetWorkThreadInterface::~NNetWorkThreadInterface( )
{
	m_pNNetWorkThread = nullptr;
	m_pTraceStream = nullptr;
}

void NNetWorkThreadInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void NNetWorkThreadInterface::Start
( 
	HWND                 const hwndApplication,
	ActionTimer        * const pActionTimer,
    EventInterface     * const pEvent,
	ObserverInterface  * const pObserver,
	SlowMotionRatio    * const pSlowMotionRatio,
	NNetModel          * const pModel,
	BOOL                 const bAsync
)
{
	m_pModel = pModel;
	m_pNNetWorkThread = new NNetWorkThread
	( 
		hwndApplication, 
		pActionTimer,
		pEvent, 
		pObserver,
		pSlowMotionRatio,
		this,
		pModel,
		bAsync
	);
}

void NNetWorkThreadInterface::Stop( )
{
	m_pNNetWorkThread->Terminate( );
	delete m_pNNetWorkThread;
	m_pNNetWorkThread = nullptr;
}

void NNetWorkThreadInterface::PostResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::RESET_TIMER), 0, 0 );
}

void NNetWorkThreadInterface::PostConnect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CONNECT), idSrc.GetValue(), idDst.GetValue() );
}

void NNetWorkThreadInterface::PostRemoveShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::REMOVE_SHAPE), id.GetValue(), 0 );
}

void NNetWorkThreadInterface::PostDisconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::DISCONNECT), id.GetValue(), 0 );
}

static WorkThreadMessage::Id const GetWorkThreadMessage( tParameter const p )
{
	static unordered_map < tParameter, NNetWorkThreadMessage::Id const > mapParam =
	{
		{ tParameter::pulseSpeed,    NNetWorkThreadMessage::Id::PULSE_SPEED       },
		{ tParameter::pulseWidth,    NNetWorkThreadMessage::Id::PULSE_WIDTH       },
		{ tParameter::signalLoss,    NNetWorkThreadMessage::Id::DAMPING_FACTOR    },
		{ tParameter::threshold,     NNetWorkThreadMessage::Id::THRESHOLD         },
		{ tParameter::peakVoltage,   NNetWorkThreadMessage::Id::PEAK_VOLTAGE      },
		{ tParameter::refractPeriod, NNetWorkThreadMessage::Id::REFRACTORY_PERIOD }
	};				  

	return static_cast< WorkThreadMessage::Id >( mapParam.at( p ) );
}

void NNetWorkThreadInterface::PostResetModel( )
{
	if ( IsTraceOn( ) )
	{
		TraceStream( ) << __func__ << endl;
	}
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::RESET_MODEL), 0, 0 );
}

void NNetWorkThreadInterface::PostSetPulseRate( ShapeId const id, float const fNewValue )
{
	if ( IsTraceOn( ) )
	{
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	}
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::PULSE_RATE), id.GetValue(), (LPARAM &)fNewValue );
}

void NNetWorkThreadInterface::PostSetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
	{
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	}
	WorkMessage( TRUE, GetWorkThreadMessage( param ), 0, (LPARAM &)fNewValue );
}

void NNetWorkThreadInterface::PostMoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::MOVE_SHAPE), id.GetValue(), Util::Pack2UINT64(delta) );
}

void NNetWorkThreadInterface::PostSlowMotionChanged( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED), 0, 0 );
}

void NNetWorkThreadInterface::PostActionCommand
(
	int     const idMsg,
	ShapeId const idShape,
	LPARAM  const lParam
)
{
	static unordered_map < int, NNetWorkThreadMessage::Id const > mapMsg =
	{
		{ IDD_INSERT_NEURON,       NNetWorkThreadMessage::Id::INSERT_NEURON       },
		{ IDD_NEW_NEURON,          NNetWorkThreadMessage::Id::NEW_NEURON          },
		{ IDD_NEW_INPUT_NEURON,    NNetWorkThreadMessage::Id::NEW_INPUT_NEURON    },
		{ IDD_APPEND_NEURON,       NNetWorkThreadMessage::Id::APPEND_NEURON       },
		{ IDD_APPEND_INPUT_NEURON, NNetWorkThreadMessage::Id::APPEND_INPUT_NEURON },
		{ IDD_ADD_OUTGOING2KNOT,   NNetWorkThreadMessage::Id::ADD_OUTGOING2KNOT   },
		{ IDD_ADD_INCOMING2KNOT,   NNetWorkThreadMessage::Id::ADD_INCOMING2KNOT   },
		{ IDD_ADD_OUTGOING2PIPE,   NNetWorkThreadMessage::Id::ADD_OUTGOING2PIPE   },
		{ IDD_ADD_INCOMING2PIPE,   NNetWorkThreadMessage::Id::ADD_INCOMING2PIPE   }
	};				  

	WorkMessage
	( 
		TRUE, 
		static_cast< WorkThreadMessage::Id >( mapMsg.at( idMsg ) ), 
		idShape.GetValue( ), 
		lParam 
	);
}

void NNetWorkThreadInterface::PostReset( BOOL bResetHistSys )
{
	if ( IsTraceOn( ) )
		* m_pTraceStream << __func__ << (bResetHistSys ? 1 : 0) << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(WorkThreadMessage::Id::RESET_MODEL), bResetHistSys, 0 );
}

void NNetWorkThreadInterface::PostGenerationStep( )
{
	if ( m_bTrace )
		* m_pTraceStream << __func__ << endl;

	Continue( );     // trigger worker thread if waiting on POI event

	WorkMessage( FALSE, WorkThreadMessage::Id::NEXT_GENERATION, 0, 0 );
}

void NNetWorkThreadInterface::PostRunGenerations( BOOL const bFirst )
{
	//if ( m_bTrace )
	//    * m_pTraceStream << L"PostGenerationStep" << endl;
	WorkMessage( FALSE, WorkThreadMessage::Id::GENERATION_RUN, 0, bFirst );
}

void NNetWorkThreadInterface::PostRepeatGenerationStep( )
{
	//if ( m_bTrace )
	//    * m_pTraceStream << L"PostGenerationStep" << endl;
	WorkMessage( FALSE, WorkThreadMessage::Id::REPEAT_NEXT_GENERATION, 0, 0 );
}

void NNetWorkThreadInterface::PostStopComputation( )
{
	WorkMessage( FALSE, WorkThreadMessage::Id::STOP, 0, 0 );
}

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

using std::endl;
using std::unordered_map;

NNetWorkThreadInterface::NNetWorkThreadInterface( ) :
	m_pNNetWorkThread( nullptr ),
	m_pModel( nullptr )
{ }

NNetWorkThreadInterface::~NNetWorkThreadInterface( )
{
	m_pNNetWorkThread = nullptr;
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

	WorkThreadInterface::Start( m_pNNetWorkThread );
}

void NNetWorkThreadInterface::Stop( )
{
	WorkThreadInterface::Stop();
	delete m_pNNetWorkThread;
}

void NNetWorkThreadInterface::PostResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::RESET_TIMER), 0, 0 );
}

void NNetWorkThreadInterface::PostConnect( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
//	WorkThreadInterface::PostStopComputation();
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CONNECT), 0, 0 );
//	WorkThreadInterface::PostRunGenerations( true );
}

void NNetWorkThreadInterface::PostTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::TRIGGER), id.GetValue(), 0 );
}

void NNetWorkThreadInterface::PostHighlight( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::HIGHLIGHT), id.GetValue(), 0 );
}

void NNetWorkThreadInterface::PostSuperHighlight( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::SUPER_HIGHLIGHT), id.GetValue(), 0 );
}

static WorkThreadMessage::Id const GetWorkThreadMessage( tParameter const p )
{
	static unordered_map < tParameter, NNetWorkThreadMessage::Id const > mapParam =
	{
		{ tParameter::pulseRate,        NNetWorkThreadMessage::Id::PULSE_RATE        },
		{ tParameter::pulseSpeed,       NNetWorkThreadMessage::Id::PULSE_SPEED       },
		{ tParameter::pulseWidth,       NNetWorkThreadMessage::Id::PULSE_WIDTH       },
		{ tParameter::dampingFactor,    NNetWorkThreadMessage::Id::DAMPING_FACTOR    },
		{ tParameter::threshold,        NNetWorkThreadMessage::Id::THRESHOLD         },
		{ tParameter::peakVoltage,      NNetWorkThreadMessage::Id::PEAK_VOLTAGE      },
		{ tParameter::refractoryPeriod, NNetWorkThreadMessage::Id::REFRACTORY_PERIOD }
	};				  

	return static_cast< WorkThreadMessage::Id >( mapParam.at( p ) );
}

void NNetWorkThreadInterface::PostSetParameter( tParameter const param, float const fNewValue, ShapeId const id )
{
	if ( IsTraceOn( ) )
	{
		TraceStream( ) << __func__ << L" " << m_pModel->GetParameterName( param ) << L" " << fNewValue << id.GetValue() << endl;
	}
	WorkMessage( TRUE, GetWorkThreadMessage( param ), id.GetValue(), (LPARAM &)fNewValue );
}

void NNetWorkThreadInterface::PostMoveShape( ShapeId const id, MicroMeterPoint const & newPos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << newPos << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::MOVE_SHAPE_TO), id.GetValue(), Util::Pack2UINT64(newPos) );
}

void NNetWorkThreadInterface::PostSlowMotionChanged( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED), 0, 0 );
}

void NNetWorkThreadInterface::PostCreateNewBranch( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CREATE_NEW_BRANCH), id.GetValue(), 0 );
}

void NNetWorkThreadInterface::PostSplitPipeline( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	auto x = pos.GetXvalue();
	auto y = pos.GetYvalue();
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::SPLIT_PIPELINE), id.GetValue(), Util::Pack2UINT64(pos));
}

void NNetWorkThreadInterface::PostCreateNewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	auto x = pos.GetXvalue();
	auto y = pos.GetYvalue();
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CREATE_NEW_NEURON), (WPARAM &)x, (LPARAM &)y );
}

void NNetWorkThreadInterface::PostCreateNewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	auto x = pos.GetXvalue();
	auto y = pos.GetYvalue();
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CREATE_NEW_INPUT_NEURON), (WPARAM &)x, (LPARAM &)y );
}

void NNetWorkThreadInterface::PostCreateNewOutputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	auto x = pos.GetXvalue();
	auto y = pos.GetYvalue();
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CREATE_NEW_OUTPUT_NEURON), (WPARAM &)x, (LPARAM &)y );
}

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
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CONNECT), 0, 0 );
}

void NNetWorkThreadInterface::PostRemoveShape( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::REMOVE_SHAPE), 0, 0 );
}

void NNetWorkThreadInterface::PostHighlight( ShapeId const id, bool const bSuper )
{
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::HIGHLIGHT), id.GetValue(), bSuper );
}

static WorkThreadMessage::Id const GetWorkThreadMessage( tParameter const p )
{
	static unordered_map < tParameter, NNetWorkThreadMessage::Id const > mapParam =
	{
		{ tParameter::pulseRate,        NNetWorkThreadMessage::Id::PULSE_RATE        },
		{ tParameter::pulseSpeed,       NNetWorkThreadMessage::Id::PULSE_SPEED       },
		{ tParameter::pulseWidth,       NNetWorkThreadMessage::Id::PULSE_WIDTH       },
		{ tParameter::signalLoss,       NNetWorkThreadMessage::Id::DAMPING_FACTOR    },
		{ tParameter::threshold,        NNetWorkThreadMessage::Id::THRESHOLD         },
		{ tParameter::peakVoltage,      NNetWorkThreadMessage::Id::PEAK_VOLTAGE      },
		{ tParameter::refractoryPeriod, NNetWorkThreadMessage::Id::REFRACTORY_PERIOD }
	};				  

	return static_cast< WorkThreadMessage::Id >( mapParam.at( p ) );
}

void NNetWorkThreadInterface::PostSetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
	{
		TraceStream( ) << __func__ << L" " << m_pModel->GetParameterName( param ) << L" " << fNewValue << endl;
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

void NNetWorkThreadInterface::PostAddNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::ADD_NEURON), 0, Util::Pack2UINT64(pos) );
}

void NNetWorkThreadInterface::PostAddInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::ADD_INPUT_NEURON), 0, Util::Pack2UINT64(pos) );
}

void NNetWorkThreadInterface::PostAddOutgoing( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::ADD_OUTGOING), 0, Util::Pack2UINT64(pos) );
}

void NNetWorkThreadInterface::PostAddIncoming( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::ADD_INCOMING), 0, Util::Pack2UINT64(pos) );
}

void NNetWorkThreadInterface::PostInsertNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::INSERT_NEURON), 0, Util::Pack2UINT64(pos) );
}

void NNetWorkThreadInterface::PostSplitPipeline( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::SPLIT_PIPELINE), 0, Util::Pack2UINT64(pos) );
}

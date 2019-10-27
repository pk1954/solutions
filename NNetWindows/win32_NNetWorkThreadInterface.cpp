// win32_NNetWorkThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include "Resource.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_script.h"
#include "NNetReadBuffer.h"
#include "win32_NNetWorkThread.h"
#include "win32_NNetWorkThreadInterface.h"

using std::endl;

NNetWorkThreadInterface::NNetWorkThreadInterface( ) :
	m_pNNetWorkThread( nullptr )
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
	NNetModel          * const pNNetModel,
	BOOL                 const bAsync
)
{
	m_pNNetWorkThread = new NNetWorkThread
	( 
		hwndApplication, 
		pActionTimer,
		pEvent, 
		pObserver,
		pSlowMotionRatio,
		this,
		pNNetModel,
		bAsync
	);

	WorkThreadInterface::Start( m_pNNetWorkThread );
}

void NNetWorkThreadInterface::Stop( )
{
	WorkThreadInterface::Stop();
	delete m_pNNetWorkThread;
}

void NNetWorkThreadInterface::PostConnect( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::CONNECT), 0, 0 );
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

void NNetWorkThreadInterface::PostPulseFrequency( ShapeId const id, fHertz const freq )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << freq.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::PULSE_FREQ), id.GetValue(), (LPARAM &)freq.GetValue() );
}

void NNetWorkThreadInterface::PostPulseSpeed( ShapeId const id, meterPerSec const speed )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << speed.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::PULSE_SPEED), id.GetValue(), (LPARAM &)speed.GetValue() );
}

void NNetWorkThreadInterface::PostMoveShape( ShapeId const id, MicroMeterPoint const newPos )
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

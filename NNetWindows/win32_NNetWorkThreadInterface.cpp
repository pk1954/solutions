// win32_NNetWorkThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include "Resource.h"
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
	NNetModel          * const pNNetModel
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
		pNNetModel
	);

	WorkThreadInterface::Start( m_pNNetWorkThread );
}

void NNetWorkThreadInterface::PostHighlight( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::HIGHLIGHT), 0, id.GetValue() );
}
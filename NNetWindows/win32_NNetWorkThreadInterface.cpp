// win32_NNetWorkThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include "Resource.h"
#include "win32_script.h"
#include "NNetModel.h"
#include "NNetReadBuffer.h"
#include "NNetHistorySysGlue.h"
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
	Delay              * const pDelay,
	ObserverInterface  * const pObserver, 
    NNetHistorySysGlue * const pNNetHistGlue
)
{
	m_pNNetWorkThread  = new NNetWorkThread
	( 
		hwndApplication, 
		pActionTimer,
		pEvent, 
		pDelay, 
		pObserver, 
		pNNetHistGlue, 
		this
	);

	WorkThreadInterface::Start( m_pNNetWorkThread );
}

void NNetWorkThreadInterface::PostRedo( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( FALSE,static_cast<WorkThreadMessage::Id>( NNetWorkThreadMessage::Id::REDO), 0, 0 );
}

void NNetWorkThreadInterface::PostUndo( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	WorkMessage( FALSE, static_cast<WorkThreadMessage::Id>(NNetWorkThreadMessage::Id::UNDO), 0, 0 );
}

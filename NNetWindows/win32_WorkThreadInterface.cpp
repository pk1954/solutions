// win32_workThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include "win32_script.h"
#include "win32_WorkThread.h"
#include "win32_WorkThreadInterface.h"

using std::wostream;
using std::wcout;
using std::endl;

WorkThreadInterface::WorkThreadInterface( ) :
	m_pWorkThread ( nullptr ),
	m_pTraceStream( nullptr ),
	m_bTrace      ( TRUE )
{ 
}

WorkThreadInterface::~WorkThreadInterface( )
{
	m_pWorkThread  = nullptr;
    m_pTraceStream = nullptr;
}

void WorkThreadInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void WorkThreadInterface::Start
( 
	WorkThread * const pWorkThread
)
{
	m_pWorkThread = pWorkThread;
}

void WorkThreadInterface::Stop( )
{
	m_pWorkThread->Terminate( );
	m_pWorkThread = nullptr;
}

// procedural interface of worker thread

void WorkThreadInterface::PostReset( BOOL bResetHistSys )
{
	if ( IsTraceOn( ) )
		* m_pTraceStream << __func__ << (bResetHistSys ? 1 : 0) << endl;
	WorkMessage( TRUE, static_cast<WorkThreadMessage::Id>(WorkThreadMessage::Id::RESET_MODEL), bResetHistSys, 0 );
}

void WorkThreadInterface::PostGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	m_pWorkThread->Continue( );     // trigger worker thread if waiting on POI event
	
	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::NEXT_GENERATION, 0, 0 );
}

void WorkThreadInterface::PostRunGenerations( BOOL const bFirst )
{
    //if ( m_bTrace )
    //    * m_pTraceStream << L"PostGenerationStep" << endl;
	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::GENERATION_RUN, 0, bFirst );
}

void WorkThreadInterface::PostRepeatGenerationStep( )
{
    //if ( m_bTrace )
    //    * m_pTraceStream << L"PostGenerationStep" << endl;
    m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::REPEAT_NEXT_GENERATION, 0, 0 );
}

void WorkThreadInterface::PostStopComputation( )
{
	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::STOP, 0, 0 );
}

// win32_workThreadInterface.cpp
//
// Toolbox/win32_utilities

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
	delete m_pWorkThread;
	m_pWorkThread = nullptr;
}

BOOL WorkThreadInterface::IsRunning( ) const
{
	return m_pWorkThread->IsRunning( );
}

void WorkThreadInterface::postGotoGeneration( HIST_GENERATION const gen )
{
    assert( gen >= 0 );

    m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::GOTO_GENERATION, 0, static_cast<LPARAM>(gen.GetLong()) );
}

// procedural interface of worker thread

HIST_GENERATION WorkThreadInterface::GetGenDemanded( ) const 
{ 
	return m_pWorkThread->GetGenDemanded( );
}

void WorkThreadInterface::WorkMessage
( 
	BOOL                  const isEditOperation,
	WorkThreadMessage::Id const msg, 
	WPARAM                const wparam, 
	LPARAM                const lparam
)
{
	m_pWorkThread->WorkMessage( isEditOperation, msg, wparam, lparam );
}

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
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::GENERATION_RUN, 0, bFirst );
}

void WorkThreadInterface::PostRepeatGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
    m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::REPEAT_NEXT_GENERATION, 0, 0 );
}

void WorkThreadInterface::PostPrevGeneration( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	m_pWorkThread->Continue( );     // trigger worker thread if waiting on POI event

	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::PREV_GENERATION, 0, 0 );
}

void WorkThreadInterface::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << gen << endl;
	postGotoGeneration( gen );
}

void WorkThreadInterface::PostStopComputation( )
{
	m_pWorkThread->WorkMessage( FALSE, WorkThreadMessage::Id::STOP, 0, 0 );
}

// no trace output

void WorkThreadInterface::TerminateThread( )
{
	m_pWorkThread->Terminate( );    // wait until thread has stopped
}

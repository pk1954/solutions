// HistAllocThread.cpp

#include "stdafx.h"
#include "win32_thread.h"
#include "HistorySystem.h"
#include "HistAllocThread.h"

HistAllocThread::HistAllocThread
( 
	HistorySystem const * const pHistSys,
	BOOL                  const bAsync      
) :
	m_pHistorySys( pHistSys )
{
	if ( bAsync )
		StartThread( FALSE, L"HistAlloc" );  // start thread without loop, ThreadStartupFunc has its own loop
	else
		ThreadStartupFunc( );
}

void HistAllocThread::ThreadStartupFunc( )
{
	Continue( );   // trigger mother thread to continue
	while (m_pHistorySys->AddHistorySlot()) {}
}

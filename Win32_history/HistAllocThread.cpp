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
	{
		StartThread( FALSE );  // start thread without loop, ThreadStartupFunc has its own loop
		SetThreadAffinityMask( 0x0004 );
	}
	else
	{
		ThreadStartupFunc( );
	}
}

void HistAllocThread::ThreadStartupFunc( )
{
	while (m_pHistorySys->AddHistorySlot()) {}
}

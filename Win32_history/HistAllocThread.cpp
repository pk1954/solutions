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
		StartThread( FALSE );  // start thread without loop, ThreadMsgDispatcher has its own loop
		SetThreadAffinityMask( 0x0003 );
	}
	else
	{
		ThreadMsgDispatcher( 0, 0, 0 );
	}
}

void HistAllocThread::ThreadStartupFunc( )
{
	while (m_pHistorySys->AddHistorySlot()) {}
}

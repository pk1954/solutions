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
		StartThread( 0x0003 );
		PostMessage( THREAD_MSG_APP_FIRST, 0, 0 );  // any msg will do, just to trigger one DispatchMessage
	}
	else
	{
		DispatchMessage( THREAD_MSG_APP_FIRST, 0, 0 );
	}
}

void HistAllocThread::DispatchMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam  )
{
	while (m_pHistorySys->AddHistorySlot()) {}
	Terminate( );  // kill yourself
}

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
		StartThread( );
		SetThreadAffinityMask( 0x0003 );
		PostMessage( THREAD_MSG_APP_FIRST, 0, 0 );  // any msg will do, just to trigger one DispatchThreadMsg
	}
	else
	{
		DispatchThreadMsg( THREAD_MSG_APP_FIRST, 0, 0 );
	}
}

LRESULT HistAllocThread::DispatchThreadMsg( UINT const message, WPARAM const wParam, LPARAM const lParam  )
{
	while (m_pHistorySys->AddHistorySlot()) {}
	Terminate( );  // kill yourself
	return 0;
}

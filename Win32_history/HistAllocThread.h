// HistAllocThread.h
//

#pragma once

#include "HistorySystem.h"
#include "win32_thread.h"

class HistorySystem;

class HistAllocThread: public Util::Thread
{
public:

	HistAllocThread::HistAllocThread
	( 
		HistorySystem const * const pHistSys,
		BOOL                  const bAsync      
	) :
	m_pHistorySys( pHistSys )
	{
		if ( bAsync )
		{
			StartThread( L"HistAlloc", true );  
		}
		else
		{
			while (m_pHistorySys->AddHistorySlot()) {}
		}
	}

	~HistAllocThread() {};

	void HistAllocThread::ThreadStartupFunc( )
	{
		ThreadMsgDispatcher( MSG{0,0,0,0} );
	}

	void HistAllocThread::ThreadMsgDispatcher( MSG msg )
	{
		if ( m_pHistorySys->AddHistorySlot() )                    // if allocation of slot 
		{                                                         // was successfull,
			PostThreadMsg( msg.message, msg.wParam, msg.lParam ); // do it again
		}
	}

private:
	HistorySystem const * m_pHistorySys;
};

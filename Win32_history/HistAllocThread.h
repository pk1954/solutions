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
			StartThread( FALSE, L"HistAlloc" );  // start thread without loop, ThreadStartupFunc has its own loop
		}
		else
		{
			ThreadStartupFunc( );
		}
	}

	~HistAllocThread() {};

	void HistAllocThread::ThreadStartupFunc( )
	{
		while (m_pHistorySys->AddHistorySlot()) {}
	}

private:
	HistorySystem const * m_pHistorySys;
};

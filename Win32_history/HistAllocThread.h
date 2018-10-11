// HistAllocThread.h
//

#pragma once

#include "win32_thread.h"

class HistorySystem;

class HistAllocThread : public Util::Thread
{
public:
	HistAllocThread( HistorySystem const * const, BOOL const );
	~HistAllocThread() {};

private:
	virtual void ThreadStartupFunc( void * );

	HistorySystem const * m_pHistorySys;
};

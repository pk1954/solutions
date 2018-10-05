// HistAllocThread.h
//

#pragma once

#include "win32_thread.h"

class HistorySystem;

class HistAllocThread
{
public:
	HistAllocThread( HistorySystem const * const, BOOL const );
	~HistAllocThread( );

private:
	HistorySystem const * m_pHistorySys;
	Util::Thread        * m_pThreadSlotAllocator;
	BOOL                  m_bContinueSlotAllocation;

	friend static unsigned int __stdcall threadProc( void * );
};

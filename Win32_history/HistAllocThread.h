// HistAllocThread.h
//

#pragma once

class HistorySystem;
class ObserverInterface;

class HistAllocThread
{
public:
	HistAllocThread( HistorySystem const * const, ObserverInterface * const, BOOL const );
	~HistAllocThread( );

private:
	HistorySystem     const * m_pHistorySys;
	ObserverInterface       * m_pObserver;

	HANDLE m_hThreadSlotAllocator;
	BOOL   m_bContinueSlotAllocation;

	friend static DWORD WINAPI threadProc(_In_ LPVOID);
};

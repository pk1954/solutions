// HistAllocThread.h
//

#pragma once

class HistorySystem;

class HistAllocThread
{
public:
	HistAllocThread( HistorySystem const * const, BOOL const );
	~HistAllocThread( );

private:
	HistorySystem const * m_pHistorySys;

	HANDLE m_hThreadSlotAllocator;
	BOOL   m_bContinueSlotAllocation;

	friend static DWORD WINAPI threadProc(_In_ LPVOID);
};

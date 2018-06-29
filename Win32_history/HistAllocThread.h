// HistAllocThread.h
//

#pragma once

class HistorySystem;

class HistAllocThread
{
public:
	void AllocateHistorySlots( HistorySystem const * const, BOOL const );
	void ExitHistAllocThread();

private:
	HistorySystem const * m_pHistorySys;

	HANDLE m_hThreadSlotAllocator;
	BOOL   m_bContinueSlotAllocation;

	friend static DWORD WINAPI threadProc(_In_ LPVOID);
};

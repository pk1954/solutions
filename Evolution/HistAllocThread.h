// HistAllocThread.h
//

#pragma once

class EvoHistorySys;

class HistAllocThread
{
public:
	void AllocateHistorySlots( EvoHistorySys const * );
	void ExitHistAllocThread();

private:
	EvoHistorySys const * m_pEvoHistorySys;

	HANDLE m_hThreadSlotAllocator;
	BOOL   m_bContinueSlotAllocation;

	friend static DWORD WINAPI threadProc(_In_ LPVOID);
};

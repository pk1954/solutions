// HistAllocThread.cpp

#include "stdafx.h"
#include "win32_util.h"
#include "EvoHistorySys.h"
#include "HistAllocThread.h"

static DWORD WINAPI threadProc(_In_ LPVOID lpParameter)
{
	HistAllocThread const * const pHistAllocThread = static_cast<HistAllocThread const *>(lpParameter);
	EvoHistorySys   const * const pHistSys = pHistAllocThread->m_pEvoHistorySys;

	(void)SetThreadAffinityMask(GetCurrentThread(), 0x0003);

	while (pHistAllocThread->m_bContinueSlotAllocation && pHistSys->AddEvoHistorySlot());

	return 0;
}

void HistAllocThread::AllocateHistorySlots( EvoHistorySys const * pHistSys )
{
	DWORD m_dwThreadId;
	m_pEvoHistorySys = pHistSys;
	m_bContinueSlotAllocation = TRUE;
	m_hThreadSlotAllocator = Util::MakeThread( threadProc, this, &m_dwThreadId, nullptr );
}

void HistAllocThread::ExitHistAllocThread()
{
	m_bContinueSlotAllocation = FALSE;
	WaitForSingleObject(m_hThreadSlotAllocator, INFINITE);
	CloseHandle(m_hThreadSlotAllocator);
}

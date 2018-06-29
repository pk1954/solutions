// HistAllocThread.cpp

#include "stdafx.h"
#include "win32_util.h"
#include "HistorySystem.h"
#include "HistAllocThread.h"

static DWORD WINAPI threadProc( _In_ LPVOID lpParameter )
{
	HistAllocThread const * const pHistAllocThread = static_cast<HistAllocThread const *>(lpParameter);
	HistorySystem   const * const pHistSys         = pHistAllocThread->m_pHistorySys;

	while ( pHistAllocThread->m_bContinueSlotAllocation && pHistSys->AddHistorySlot( ) );

	return 0;
}

void HistAllocThread::AllocateHistorySlots
( 
	HistorySystem const * const pHistSys,
	BOOL                  const bAsync      
)
{
	m_pHistorySys = pHistSys;
	m_bContinueSlotAllocation = TRUE;

	if ( bAsync )
	{
		DWORD m_dwThreadId;
		m_hThreadSlotAllocator = Util::MakeThread( threadProc, this, &m_dwThreadId, nullptr );
		(void)SetThreadAffinityMask( m_hThreadSlotAllocator, 0x0003 );
	}
	else
		threadProc( this );
}

void HistAllocThread::ExitHistAllocThread( )
{
	m_bContinueSlotAllocation = FALSE;
	WaitForSingleObject (m_hThreadSlotAllocator, INFINITE );
	CloseHandle( m_hThreadSlotAllocator );
}

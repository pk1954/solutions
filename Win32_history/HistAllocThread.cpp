// HistAllocThread.cpp

#include "stdafx.h"
#include "win32_util.h"
#include "observerInterface.h"
#include "HistorySystem.h"
#include "HistAllocThread.h"

HistAllocThread::HistAllocThread
( 
	HistorySystem     const * const pHistSys,
	ObserverInterface       * const pObserver,
	BOOL                      const bAsync      
) :
	m_pHistorySys( pHistSys ),
	m_pObserver( pObserver ),
	m_hThreadSlotAllocator( nullptr ),
	m_bContinueSlotAllocation( TRUE )
{
	if ( bAsync )
	{
		DWORD m_dwThreadId;
		m_hThreadSlotAllocator = Util::MakeThread( threadProc, this, &m_dwThreadId, nullptr );
		(void)SetThreadAffinityMask( m_hThreadSlotAllocator, 0x0003 );
	}
	else
		threadProc( this );
}

HistAllocThread::~HistAllocThread( )
{
	m_bContinueSlotAllocation = FALSE;
	if ( m_hThreadSlotAllocator != nullptr )
	{
		WaitForSingleObject (m_hThreadSlotAllocator, INFINITE );
		CloseHandle( m_hThreadSlotAllocator );
	}
}

static DWORD WINAPI threadProc( _In_ LPVOID lpParameter )
{
	HistAllocThread   const * const pHistAllocThread = static_cast<HistAllocThread const *>(lpParameter);
	HistorySystem     const * const pHistSys         = pHistAllocThread->m_pHistorySys;
	ObserverInterface       * const pObserver        = pHistAllocThread->m_pObserver;

	while (pHistAllocThread->m_bContinueSlotAllocation && pHistSys->AddHistorySlot())
	{
		if ( pObserver != nullptr )
			pObserver->SetDirtyFlag();
	}

	return 0;
}

// HistAllocThread.cpp

#include "stdafx.h"
#include "win32_thread.h"
#include "HistorySystem.h"
#include "HistAllocThread.h"

HistAllocThread::HistAllocThread
( 
	HistorySystem const * const pHistSys,
	BOOL                  const bAsync      
) :
	m_pHistorySys( pHistSys ),
	m_pThreadSlotAllocator( nullptr ),
	m_bContinueSlotAllocation( TRUE )
{
	if ( bAsync )
	{
		m_pThreadSlotAllocator = new Util::Thread( threadProc, this );
		m_pThreadSlotAllocator->SetThreadAffinityMask( 0x0003 );
	}
	else
		threadProc( this );
}

HistAllocThread::~HistAllocThread( )
{
	m_bContinueSlotAllocation = FALSE;
	if ( m_pThreadSlotAllocator != nullptr )
	{
		m_pThreadSlotAllocator->Wait4Termination( );
	}
}

static unsigned int __stdcall threadProc( void * data ) 
{
	HistAllocThread const * const pHistAllocThread = static_cast<HistAllocThread const *>(data);
	HistorySystem   const * const pHistSys         = pHistAllocThread->m_pHistorySys;

	for (;;)
	{
		if ( ! pHistAllocThread->m_bContinueSlotAllocation )
			break;
		if ( ! pHistSys->AddHistorySlot() )
			break;
	}

	return 0;
}

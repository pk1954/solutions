// EvoWindows/win32_readBuffer.h : 
//
// Handle read access from UI threads to EvolutionCore
//
// A read only copy of Evolutioncore is present in * m_pCore4Display 
// UI threads acquire non exclusive read access to * m_pCore4Display by calling LockReadBuffer and relase it by ReleaseReadBuffer.
// Worker thread computes new generations of EvolutionCore in * m_pCoreWork and tries to copy them into * m_pCore4Display.
// Copy operation is possible only if * m_pCore4Display is not locked by one or several reader threads.
// As uninterrupted operation of worker thread has higher priority, the worker thread simply continues 
// and tries to update the read buffer later, if the read buffer is locked.

#pragma once

#include "synchapi.h"
#include "EvolutionCore.h"
#include "observerInterface.h"
#include "ViewCollection.h"

class ReadBuffer : public ObserverInterface
{
public:
	ReadBuffer( ) : 
		m_pCoreWork( nullptr ),
		m_pCore4Display( nullptr )
	{
		InitializeSRWLock( & m_SRWLock );
	}

	~ReadBuffer( ) { }

	void Initialize
	(
		EvolutionCore const * pCoreWork, 
		EvolutionCore       * pCore4Display 
	)
	{
		m_pCoreWork     = pCoreWork;
		m_pCore4Display = pCore4Display;
	}

	// called by consumer threads

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void Stop( )
	{
		m_observers.Clear();
	}

	EvolutionCore const * LockReadBuffer( ) 
	{
		if ( m_pCore4Display )
			AcquireSRWLockShared( & m_SRWLock );
		return m_pCore4Display;
	}

	void ReleaseReadBuffer( )
	{
		ReleaseSRWLockShared( & m_SRWLock );
	}

	// called by producer thread

	virtual void Notify( bool const bImmediate )
	{
		if ( bImmediate )
		{
			AcquireSRWLockExclusive( & m_SRWLock );               // if locked by readers, wait
		}
		else if ( ! TryAcquireSRWLockExclusive( & m_SRWLock ))    // if buffer is locked by readers
		{                                                         // just continue your work. 
			return;                                               // readers can synchronize with
		}														  // later version

		m_pCore4Display->CopyEvolutionCoreData( m_pCoreWork );  
		ReleaseSRWLockExclusive( & m_SRWLock );                  
		m_observers.NotifyAll( bImmediate );                     
	}

private:
	SRWLOCK               m_SRWLock;
	ViewCollection        m_observers;
	EvolutionCore       * m_pCore4Display;
	EvolutionCore const * m_pCoreWork;
};
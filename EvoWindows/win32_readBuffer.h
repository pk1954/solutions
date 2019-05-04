// win32_readBuffer.h : 
//
// EvoWindows

#pragma once

#include "synchapi.h"
#include "EvolutionCore.h"
#include "observerInterface.h"

class ReadBuffer : public ObserverInterface
{
public:
	ReadBuffer( ObserverInterface * pObservers ) : 
		m_pObservers( pObservers ),
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

	EvolutionCore const * LockReadBuffer( ) 
	{
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
		if (TryAcquireSRWLockExclusive( & m_SRWLock ))              // if buffer is locked by readers
		{                                                           // don't let you get stopped.
			m_pCore4Display->CopyEvolutionCoreData( m_pCoreWork );  // just continue your work. 
			ReleaseSRWLockExclusive( & m_SRWLock );                 // readers can synchronize with 
			m_pObservers->Notify( bImmediate );                     // later version
		}
	}

private:
	SRWLOCK               m_SRWLock;
	ObserverInterface   * m_pObservers;
	EvolutionCore       * m_pCore4Display;
	EvolutionCore const * m_pCoreWork;
};
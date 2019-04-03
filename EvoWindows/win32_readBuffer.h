// win32_readBuffer.h : 
//
// EvoWindows

#pragma once

#include "synchapi.h"
#include "EvolutionCore.h"
#include "win32_viewCollection.h"

class ReadBuffer: public ViewCollection
{
public:
	ReadBuffer( )
		: m_pCoreWork( nullptr )
	{
		m_pCore4Display = EvolutionCore::CreateCore();
		InitializeSRWLock( & m_SRWLock );
	}

	void SetWorkCore(EvolutionCore const * pCoreWork)
	{
		m_pCoreWork = pCoreWork;
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

	void Notify( bool const bImmediate )
	{
		if (TryAcquireSRWLockExclusive( & m_SRWLock ))              // if buffer is locked by readers
		{                                                           // don't let you get stopped.
			m_pCore4Display->CopyEvolutionCoreData( m_pCoreWork );  // just continue your work. 
			ReleaseSRWLockExclusive( & m_SRWLock );                 // readers can synchronize with 
			ViewCollection::Notify( bImmediate );                   // later version
		}
	}

private:
	SRWLOCK               m_SRWLock;
	EvolutionCore       * m_pCore4Display;
	EvolutionCore const * m_pCoreWork;
};
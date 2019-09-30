// win32_readBuffer.h : 
//
// Win32_appFramework
//
// Handle read access from UI threads to Model
//
// A read only copy of Model is present in * m_pModel4Display 
// UI threads acquire non exclusive read access to * m_pModel4Display 
// by calling LockReadBuffer and relase it by ReleaseReadBuffer.
//
// Worker thread computes new generations of Model in * m_pModelWork 
// and tries to copy them into * m_pModel4Display.
// Copy operation is possible only if * m_pModel4Display is not locked by one 
// or several reader threads. As uninterrupted operation of worker thread has 
// higher priority, the worker thread simply continues and tries to update 
// the read buffer later, if the read buffer is locked.

#pragma once

#include "synchapi.h"
#include "observable.h"
#include "observerInterface.h"

template <typename MODEL>
class ReadBuffer : public ObserverInterface, public Observable
{
public:
	ReadBuffer( ) : 
		m_pModelWork( nullptr ),
		m_pModel4Display( nullptr )
	{
		InitializeSRWLock( & m_SRWLock );
	}

	~ReadBuffer( ) { }

	void Initialize
	(
		MODEL const * pModelWork, 
		MODEL       * pModel4Display 
	)
	{
		m_pModelWork     = pModelWork;
		m_pModel4Display = pModel4Display;
	}

	// called by consumer threads

	MODEL const * LockReadBuffer( ) 
	{
		if ( m_pModel4Display )
			AcquireSRWLockShared( & m_SRWLock );
		return m_pModel4Display;
	}

	void ReleaseReadBuffer( )
	{
		ReleaseSRWLockShared( & m_SRWLock );
	}

	// called by producer thread

	virtual void Notify( bool const bRunning )
	{
		if ( ! bRunning )                                         // In normal (not running) mode
		{                                                         // acquire lock definitely
			AcquireSRWLockExclusive( & m_SRWLock );               // if locked by readers, wait
		}
		else if ( ! TryAcquireSRWLockExclusive( & m_SRWLock ))    // In running mode, try to aquire lock. 
		{                                                         // If buffer is locked by readers,
			return;                                               // just continue your work.
		}														  // Readers can synchronize with later version

		m_pModel4Display->CopyModelData( m_pModelWork );  
		ReleaseSRWLockExclusive( & m_SRWLock );
		NotifyAll( ! bRunning );   // If not running, readers should update immediately    
	}

private:
	SRWLOCK       m_SRWLock;
	MODEL       * m_pModel4Display;
	MODEL const * m_pModelWork;
};
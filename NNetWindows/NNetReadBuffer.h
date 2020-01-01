// NNetReadBuffer.h 
//
// NNetWindows

#pragma once

#include "synchapi.h"
#include "observable.h"
#include "observerInterface.h"
#include "NNetModel.h"

class NNetReadBuffer : public ObserverInterface,  // Observes producer thread
	public Observable          // Can be observed by consumer threads
{
public:
	NNetReadBuffer( BOOL  const   bCopyModel ) : 
		m_bCopyModel( bCopyModel ),
		m_pModelWork( nullptr ),
		m_pModel4Display( nullptr )
	{
		if ( m_bCopyModel )
			InitializeSRWLock( & m_SRWLock );
	}

	~NNetReadBuffer( ) { }

	void Initialize
	(
		NNetModel const * pModelWork, 
		NNetModel       * pModel4Display 
	)
	{
		m_pModelWork     = pModelWork;
		m_pModel4Display = pModel4Display;
		assert( m_pModelWork != nullptr );
		if ( m_bCopyModel )
			assert( m_pModel4Display != m_pModelWork );
		else
			assert( m_pModel4Display == m_pModelWork );
	}

	// called by consumer threads

	NNetModel * GetModel( )     // only allowed in non double buffered mode          
	{ 
		assert( ! m_bCopyModel ); 
		return m_pModel4Display;            
	}                                              

	// called by producer thread

	virtual void Notify( bool const bRunMode )  // In run mode, performance of producer thread has top priority
	{                                           // immediate refresh of UI is secondary 
		bool bImmediate = ! bRunMode;         
		if ( m_bCopyModel )                     
		{
			if ( bImmediate )                                       // In immediate mode
			{                                                       // acquire lock definitely
				AcquireSRWLockExclusive( & m_SRWLock );             // if locked by readers, wait
			}
			else if ( ! TryAcquireSRWLockExclusive( & m_SRWLock ))  // In high frequency mode, try to aquire lock. 
			{                                                       // If buffer is locked by readers,
				return;                                             // just continue your work.
			}														// Readers can synchronize with later version

			m_pModel4Display->CopyModelData( m_pModelWork );        // We have exclusive access, copy Model
			ReleaseSRWLockExclusive( & m_SRWLock );                 // Release lock
		}

		NotifyAll( bImmediate );                            // Notify Observers  
	}

private:
	BOOL          m_bCopyModel;  // TRUE: 2 model instances are used, copy for read access, synchronisation with SRW locks
	SRWLOCK       m_SRWLock;
	NNetModel       * m_pModel4Display;
	NNetModel const * m_pModelWork;
};

// ReadBuffer.h : 
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

module;

//#include "synchapi.h"  // windows

export module ReadBuffer;

import Debug;
import Observable;
import ObserverInterface;
import WinBasics;

template <typename MODEL>
class ReadBuffer : public ObserverInterface,  // Observes producer thread
	               public Observable          // Can be observed by consumer threads
{
public:
	ReadBuffer(bool  const   bCopyModel) : 
		m_bCopyModel(bCopyModel),
		m_pModelWork(nullptr),
		m_pModel4Display(nullptr)
	{
		if (m_bCopyModel)
			InitializeSRWLock(& m_SRWLock);
	}

	~ReadBuffer() { }

	void Initialize
	(
		MODEL const * pModelWork, 
		MODEL       * pModel4Display 
	)
	{
		m_pModelWork     = pModelWork;
		m_pModel4Display = pModel4Display;
		Assert(m_pModelWork != nullptr);
		if (m_bCopyModel)
			Assert(m_pModel4Display != m_pModelWork);
		else
			Assert(m_pModel4Display == m_pModelWork);
	}

	// called by consumer threads

	MODEL const * GetModel()     // only allowed in non double buffered mode          
	{ 
		Assert(! m_bCopyModel); 
		return m_pModel4Display;            
	}                                              

	MODEL const * LockReadBuffer()                // Consumers call LockReadBuffer 
	{                                              // for read access to model
		if (m_pModel4Display)                    // and call RealeaseReadBuffer afterwards
			AcquireSRWLockShared(& m_SRWLock);   // The MODEL pointer provided by LockReadBuffer
		return m_pModel4Display;                   // is no longer valid after ReleaseReadBuffer call
	}                                              

	void ReleaseReadBuffer()
	{
		ReleaseSRWLockShared(& m_SRWLock);
	}

	// called by producer thread

	virtual void Notify(bool const bRunMode)  // In run mode, performance of producer thread has top priority
	{                                           // immediate refresh of UI is secondary 
		bool bImmediate = ! bRunMode;         
		if (m_bCopyModel)                     
		{
			if (bImmediate)                                       // In immediate mode
			{                                                       // acquire lock definitely
				AcquireSRWLockExclusive(& m_SRWLock);             // if locked by readers, wait
			}
			else if (! TryAcquireSRWLockExclusive(& m_SRWLock))  // In high frequency mode, try to aquire lock. 
			{                                                       // If buffer is locked by readers,
				return;                                             // just continue your work.
			}														// Readers can synchronize with later version

			m_pModel4Display->CopyModelData(m_pModelWork);        // We have exclusive access, copy Model
			ReleaseSRWLockExclusive(& m_SRWLock);                 // Release lock
		}

		NotifyAll(bImmediate);                            // Notify Observers  
	}

private:
	bool          m_bCopyModel;  // TRUE: 2 model instances are used, copy for read access, synchronisation with SRW locks
	SRWLOCK       m_SRWLock;
	MODEL       * m_pModel4Display;
	MODEL const * m_pModelWork;
};
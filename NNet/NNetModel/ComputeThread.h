// ComputeThread.h 
//
// NNetModel

#pragma once

//#include "synchapi.h"
#include <string>
#include "win32_thread.h"

class NNetModel;

class ComputeThread : public Util::Thread
{
public:
	ComputeThread
	( 
		NNetModel    * pModel, 
		unsigned int   uiThreadNumber, 
		HANDLE       * pMutex,
		SRWLOCK      * pSRWLockStartWorking,
		SRWLOCK      * pSRWLockStarted,
		SRWLOCK      * pSRWLockFinished
	) :
		m_pModelWriterInterface( pModel ),
		m_pMutex( pMutex ),
		m_pSRWLockStartWorking( pSRWLockStartWorking ),
		m_pSRWLockStarted     ( pSRWLockStarted ),
		m_pSRWLockFinished    ( pSRWLockFinished )
	{
		wstring threadName { L"Compute" };
		threadName += std::to_wstring( uiThreadNumber );
		StartThread( threadName, true );
	}

private:
	NNetModel * m_pModelWriterInterface;
	HANDLE    * m_pMutex;
	SRWLOCK   * m_pSRWLockStartWorking;
	SRWLOCK   * m_pSRWLockStarted;
	SRWLOCK   * m_pSRWLockFinished;

	virtual void ThreadMsgDispatcher( MSG const ) {};
	virtual void ThreadStartupFunc( );
};

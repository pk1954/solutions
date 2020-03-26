// ComputeThread.h 
//
// NNetModel

#pragma once

#include "synchapi.h"
#include "win32_thread.h"

class NNetModel;

class ComputeThread : public Util::Thread
{
public:
	ComputeThread
	( 
		NNetModel          * pModel, 
		int                  iStart, 
		int                  iStep,
		CONDITION_VARIABLE * pConditionVar,
		SRWLOCK            * pSRWLock
	) :
		m_pModel( pModel ),
		m_iStart( iStart ),
		m_iStep ( iStep ),
		m_pConditionVar( pConditionVar ),
		m_pSRWLock( pSRWLock )
	{
		StartThread( L"Compute", true );
	}

private:
	NNetModel          * m_pModel;
	int                  m_iStart;
	int                  m_iStep;
	CONDITION_VARIABLE * m_pConditionVar;
	SRWLOCK            * m_pSRWLock;

	virtual void ThreadMsgDispatcher( MSG const ) {};
	virtual void ThreadStartupFunc( );
};

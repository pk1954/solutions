// ComputeThread.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "ComputeThread.h"

void ComputeThread::ThreadStartupFunc( )
{
	while ( true  )
	{
		// SRWLock is locked. Condition variable is locked
		AcquireSRWLockShared( m_pSRWLock );  // wait until SRWLock is unlocked
		m_pModel->Apply2AllWithSteps( m_iStart, m_iStep, [&]( Shape & shape ) { shape.Prepare( ); } );
		// Unlock SRWLock, wait on condition variable
		SleepConditionVariableSRW( m_pConditionVar, m_pSRWLock, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED );
		// condition variable is unlocked

		// SRWLock is locked. Condition variable is locked
		AcquireSRWLockShared( m_pSRWLock );  // wait until SRWLock is unlocked
		m_pModel->Apply2AllWithSteps( m_iStart, m_iStep, [&]( Shape & shape ) { shape.Step( ); } );
		// Unlock SRWLock, wait on condition variable
		SleepConditionVariableSRW( m_pConditionVar, m_pSRWLock, INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED );
		// condition variable is unlocked
	}
}

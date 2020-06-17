// ComputeThread.cpp
//
// NNetSimu

#include "stdafx.h"
#include "NNetModel.h"
#include "SlowMotionRatio.h"
#include "win32_fatalError.h"
#include "ComputeThread.h"

void ComputeThread::Start
(
	NNetModel       * const pModel,
	Param           * const pParam,
	SlowMotionRatio * const pSlowMotionRatio,
	Observable      * const pRunObservable,
	Observable      * const pPerformanceObservable
) 
{
	m_pModel                 = pModel;
	m_pParam                 = pParam;
	m_pRunObservable         = pRunObservable;
	m_pPerformanceObservable = pPerformanceObservable;
	m_pSlowMotionRatio       = pSlowMotionRatio;
	Reset( );
	AcquireSRWLockExclusive( & m_srwlStopped );
	BeginThread( L"ComputeThread" ); 
}

void ComputeThread::ReleaseComputationLock( )
{
	if ( m_bComputationLocked )
	{
		m_bComputationLocked = false;
		if ( ! m_bStopped )  // both locks are released. We can start to run
			runComputation();
	}
}

void ComputeThread::LockComputation( )
{
	if ( ! m_bComputationLocked )
	{
		m_bComputationLocked = true;
		if ( ! m_bStopped )      // if not already stopped, stop now
			stopComputation();
	}
}

void ComputeThread::RunComputation( )
{
	if ( m_bStopped )
	{
		m_bStopped = false;
		if ( ! m_bComputationLocked )  // both locks are released. We can start to run
			runComputation();
	}
}

void ComputeThread::StopComputation( )
{
	if ( ! m_bStopped )
	{
		m_bStopped = true;
		if ( ! m_bComputationLocked )  // if not already stopped, stop now
			stopComputation();
	}
}

void ComputeThread::runComputation( )
{
	m_pRunObservable->NotifyAll( false);
	Reset( );
	ReleaseSRWLockExclusive( & m_srwlStopped ); // allow ComputeThread to run
}

void ComputeThread::stopComputation( )
{
	AcquireSRWLockExclusive( & m_srwlStopped ); // wait until ComputeThread has finished activities
	m_pRunObservable->NotifyAll( false );
	m_hrTimer.Stop();
}

void ComputeThread::ThreadStartupFunc( )  // everything happens in startup function
{                                         // no thread messages used
	for (;;)
	{
		AcquireSRWLockExclusive( & m_srwlStopped );

		while ( ! (m_bStopped || m_bComputationLocked) )
		{
			fMicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
			fMicroSecs const usNominalSimuTime  { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
			fMicroSecs const usActualSimuTime   { m_pModel->GetSimulationTime( ) };                                // get actual time stamp
			fMicroSecs const usMissingSimuTime  { usNominalSimuTime - usActualSimuTime };                          // compute missing simulation time
			long       const lCycles            { CastToLong(usMissingSimuTime / m_pParam->GetTimeResolution()) }; // compute # cycles to be computed
			long       const lCyclesTodo        { max( 0, lCycles ) };
			long             lCyclesDone        { 0 };
			while ( (lCyclesDone < lCyclesTodo) && ! m_bStopped )
			{
				if ( m_pModel->Compute( ) ) // returns true, if stop on trigger fires
				{
					m_bStopped = true;
					m_pRunObservable->NotifyAll( false );
				}
				++lCyclesDone;
			}

			fMicroSecs const usSpentInCompute { m_hrTimer.GetMicroSecsTilStart( ) - usTilStartRealTime };
			if ( lCyclesDone > 0 )
			{
				m_usRealTimeSpentPerCycle = usSpentInCompute / CastToFloat(lCyclesDone);
				m_pPerformanceObservable->NotifyAll( false);
			}

			fMicroSecs const usSleepTime { m_usRealTimeAvailPerCycle - usSpentInCompute };
			if ( usSleepTime > 10000.0_MicroSecs )
				Sleep( 10 );
		}
		ReleaseSRWLockExclusive( & m_srwlStopped );
	}
}

void ComputeThread::Reset( )
{
	m_usRealTimeAvailPerCycle = m_pSlowMotionRatio->SimuTime2RealTime( GetSimuTimeResolution() );
	m_hrTimer.Restart();
	m_pModel->ResetSimulationTime();
}

void ComputeThread::SingleStep( ) 
{ 
	m_pModel->Compute( );
}

fMicroSecs ComputeThread::GetSimulationTime( ) const 
{ 
	return m_pModel->GetSimulationTime( ); 
}

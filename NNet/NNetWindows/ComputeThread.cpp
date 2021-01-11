// ComputeThread.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetModel.h"
#include "Observable.h"
#include "SlowMotionRatio.h"
#include "win32_fatalError.h"
#include "NNetParameters.h"
#include "ComputeThread.h"

void ComputeThread::Start
(
	NNetModel       * const pModel,
	SlowMotionRatio * const pSlowMotionRatio,
	Observable      * const pRunObservable,
	Observable      * const pPerformanceObservable,
	Observable      * const pDynamicModelObservable
) 
{
	m_pModel                  = pModel;
	m_pRunObservable          = pRunObservable;
	m_pPerformanceObservable  = pPerformanceObservable;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pSlowMotionRatio        = pSlowMotionRatio;
	m_pModel->SetSimulationTime( );
	m_pPerformanceObservable->NotifyAll( false );
	reset( );
	AcquireSRWLockExclusive( & m_srwlStopped );
	BeginThread( L"ComputeThread" ); 
}

void ComputeThread::Notify( bool const bImmediate ) // slowmo ratio or parameters have changed
{
	reset( );
	m_pRunObservable->NotifyAll( false);
}

void ComputeThread::reset( )
{
	LockComputation( );
	m_usSimuTimeAtLastReset = m_pModel->GetSimulationTime( );
	m_ticksNetRunning       = Ticks( 0 );
	m_ticksAtLastRun        = m_hrTimer.ReadHiResTimer();
	m_usSimuTimeResolution  = m_pModel->GetParams().GetTimeResolution(); 
	m_usTimeAvailPerCycle   = m_pSlowMotionRatio->SimuTime2RealTime( m_usSimuTimeResolution ); 
	ReleaseComputationLock( );
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
	m_ticksAtLastRun = m_hrTimer.ReadHiResTimer();
	m_pRunObservable->NotifyAll( false);
	ReleaseSRWLockExclusive( & m_srwlStopped ); // allow ComputeThread to run
}

void ComputeThread::stopComputation( )
{
	AcquireSRWLockExclusive( & m_srwlStopped ); // wait until ComputeThread has finished activities
	m_ticksNetRunning += m_hrTimer.ReadHiResTimer() - m_ticksAtLastRun;
	m_pRunObservable->NotifyAll( false );
}

void ComputeThread::ThreadStartupFunc( )  // everything happens in startup function
{                                         // no thread messages used
	for (;;)
	{
		AcquireSRWLockExclusive( & m_srwlStopped );

		while ( ! (m_bStopped || m_bComputationLocked) )
		{
			fMicroSecs const usNominalSimuTime { m_pSlowMotionRatio->RealTime2SimuTime( netRealTimeSinceLastReset( ) ) };
			fMicroSecs const usMissingSimuTime { usNominalSimuTime - simuTimeSinceLastReset() };           // compute missing simulation time
			long       const lCycles           { Cast2Long(usMissingSimuTime / m_usSimuTimeResolution) }; // compute # cycles to be computed
			long       const lCyclesTodo       { max( 0, lCycles ) };
			long             lCyclesDone       { 0 };

			Ticks const ticksBeforeLoop { m_hrTimer.ReadHiResTimer() };
			while ( (lCyclesDone < lCyclesTodo) && ! (m_bStopped || m_bComputationLocked) )
			{
				if ( m_pModel->Compute( ) ) // returns true, if stop on trigger fires
				{
					m_bStopped = true;
					m_pRunObservable->NotifyAll( false ); // notify observers, that computation stopped
				}
				m_pDynamicModelObservable->NotifyAll( false );
				++lCyclesDone;
			}
			Ticks const ticksInLoop { m_hrTimer.ReadHiResTimer() - ticksBeforeLoop };

			if ( lCyclesDone > 0 )
			{
				m_usRealTimeSpentPerCycle = m_hrTimer.TicksToMicroSecs( ticksInLoop / lCyclesDone );
				m_fEffectiveSlowMo        = netRealTimeSinceLastReset( ) / simuTimeSinceLastReset();
				m_pPerformanceObservable->NotifyAll( false);
			}

			fMicroSecs const usSpentInCompute { m_hrTimer.TicksToMicroSecs(ticksInLoop) };
			fMicroSecs const usSleepTime { m_usTimeAvailPerCycle - usSpentInCompute };
			if ( usSleepTime > 10000.0_MicroSecs )
				Sleep( 10 );
		}

		ReleaseSRWLockExclusive( & m_srwlStopped );
	}
}

void ComputeThread::SingleStep( ) 
{ 
	m_pModel->Compute( );
	m_pDynamicModelObservable->NotifyAll( false );
}

fMicroSecs const ComputeThread::simuTimeSinceLastReset()
{ 
	return m_pModel->GetSimulationTime() - m_usSimuTimeAtLastReset; 
};

fMicroSecs const ComputeThread::netRealTimeSinceLastReset( )
{
	Ticks      ticksNet      { m_ticksNetRunning - m_ticksAtLastRun + m_hrTimer.ReadHiResTimer() };
	fMicroSecs usNetRealTime { m_hrTimer.TicksToMicroSecs( ticksNet ) };
	return usNetRealTime;
}

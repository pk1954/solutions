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
	m_pModel->SetSimulationTime( );
	reset( );
	AcquireSRWLockExclusive( & m_srwlStopped );
	BeginThread( L"ComputeThread" ); 
}

void ComputeThread::Notify( bool const bImmediate ) // slowmo ratio or parameters have changed
{
	reset( );
}

void ComputeThread::reset( )
{
	LockComputation( );
	m_usSimuTimeAtLastReset = m_pModel->GetSimulationTime( );
	m_ticksNetRunning       = Ticks( 0 );
	m_ticksAtLastRun        = m_hrTimer.ReadHiResTimer();
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
		Ticks ticks { m_hrTimer.ReadHiResTimer() };

		while ( ! (m_bStopped || m_bComputationLocked) )
		{
			Ticks      const ticksBeforeLoop          { m_hrTimer.ReadHiResTimer() };
			Ticks            ticksNet                 { m_ticksNetRunning + ticksBeforeLoop - m_ticksAtLastRun };
			fMicroSecs       usNetRealTime            { m_hrTimer.TicksToMicroSecs( ticksNet ) };
			fMicroSecs const usNominalSimuTime        { m_pSlowMotionRatio->RealTime2SimuTime( usNetRealTime ) };
			fMicroSecs const usSimuTimeSinceLastReset { m_pModel->GetSimulationTime( ) - m_usSimuTimeAtLastReset };
			if ( usNominalSimuTime < usSimuTimeSinceLastReset )
			{
				int x = 42;
			}
			assert( usNominalSimuTime >= usSimuTimeSinceLastReset );                       
			fMicroSecs const usMissingSimuTime { usNominalSimuTime - usSimuTimeSinceLastReset };                  // compute missing simulation time
			long       const lCycles           { CastToLong(usMissingSimuTime / m_pParam->GetTimeResolution()) }; // compute # cycles to be computed
			long       const lCyclesTodo       { max( 0, lCycles ) };
			long             lCyclesDone       { 0 };
			while ( (lCyclesDone < lCyclesTodo) && ! (m_bStopped || m_bComputationLocked) )
			{
				if ( m_pModel->Compute( ) ) // returns true, if stop on trigger fires
				{
					m_bStopped = true;
					m_pRunObservable->NotifyAll( false ); // notify observers, that computation stopped
				}
				++lCyclesDone;
			}
	
			Ticks      const ticksAfterLoop   { m_hrTimer.ReadHiResTimer() };
			Ticks      const ticksInLoop      { ticksAfterLoop - ticksBeforeLoop };
			fMicroSecs const usSpentInCompute { m_hrTimer.TicksToMicroSecs(ticksInLoop) };
			ticksNet      = m_ticksNetRunning + ticksAfterLoop - m_ticksAtLastRun;
			usNetRealTime = m_hrTimer.TicksToMicroSecs( ticksNet );
			if ( lCyclesDone > 0 )
			{
				fMicroSecs usSimuTimeSinceLastReset { m_pModel->GetSimulationTime() - m_usSimuTimeAtLastReset };
				m_fEffectiveSlowMo        = usNetRealTime / usSimuTimeSinceLastReset;
				m_usRealTimeSpentPerCycle = usSpentInCompute / CastToFloat(lCyclesDone);
				m_pPerformanceObservable->NotifyAll( false);
			}

			fMicroSecs const usSleepTime { GetTimeAvailPerCycle() - usSpentInCompute };
			if ( usSleepTime > 10000.0_MicroSecs )
				Sleep( 10 );
		}

		ReleaseSRWLockExclusive( & m_srwlStopped );
	}
}

void ComputeThread::SingleStep( ) 
{ 
	m_pModel->Compute( );
}

fMicroSecs ComputeThread::GetTimeAvailPerCycle( ) const 
{ 
	return m_pSlowMotionRatio->SimuTime2RealTime( GetSimuTimeResolution() ); 
};

fMicroSecs ComputeThread::GetSimuTimeResolution( ) const 
{ 
	return m_pParam->GetTimeResolution(); 
}

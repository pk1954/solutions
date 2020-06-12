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
	StartThread( L"ComputeThread", true ); 
}

void ComputeThread::RunComputation( )
{
	if ( ! m_bContinue )
	{
		m_bContinue = true;
		m_pRunObservable->NotifyAll( false);
		Reset( );
		m_runEvent.Continue();
	}
}

void ComputeThread::StopComputation( )
{
	if ( m_bContinue )
	{
		m_bContinue = false;
		m_pRunObservable->NotifyAll( false );
		m_hrTimer.Stop();
		while ( ! m_bWaiting )  // wait until ComputeThread has finished activities
			Sleep( 1 );
	}
}

void ComputeThread::ThreadStartupFunc( )  // everything happens in startup function
{                                         // no thread messages used
	for (;;)
	{
		m_runEvent.Wait( );
		m_bWaiting = false;
		while ( m_bContinue )
			compute();
		m_bWaiting = true;
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

void ComputeThread::compute() 
{
	fMicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
	fMicroSecs const usNominalSimuTime  { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
	fMicroSecs const usActualSimuTime   { m_pModel->GetSimulationTime( ) };                                // get actual time stamp
	fMicroSecs const usMissingSimuTime  { usNominalSimuTime - usActualSimuTime };                          // compute missing simulation time
	long       const lCycles            { CastToLong(usMissingSimuTime / m_pParam->GetTimeResolution()) }; // compute # cycles to be computed
	long       const lCyclesTodo        { max( 0, lCycles ) };
	long             lCyclesDone        { 0 };
	while ( (lCyclesDone < lCyclesTodo) && m_bContinue )
	{
		if ( m_pModel->Compute( ) ) // returns true, if stop on trigger fires
			m_bContinue = false;
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

fMicroSecs ComputeThread::GetSimulationTime( ) const 
{ 
	return m_pModel->GetSimulationTime( ); 
}

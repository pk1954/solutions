// ComputeThread.cpp
//
// NNetSimu

#include "stdafx.h"
#include "NNetModel.h"
#include "SlowMotionRatio.h"
#include "win32_fatalError.h"
#include "ComputeThread.h"

class ComputeThread::TimeResObserver : public ObserverInterface
{
public:
	TimeResObserver( ComputeThread * const pComputeThread )
		: m_pThread( pComputeThread )
	{}

	virtual void Notify( bool const bImmediate )
	{
		m_pThread->m_usRealTimeAvailPerCycle = m_pThread->m_pSlowMotionRatio->SimuTime2RealTime( m_pThread->GetSimuTimeResolution() );
		m_pThread->Reset( );
	}

private:
	ComputeThread * const m_pThread;
};

ComputeThread::ComputeThread( )
{
	m_pTimeResObserver = new TimeResObserver( this );
}

ComputeThread::~ComputeThread( )
{
	delete m_pTimeResObserver;
	m_pTimeResObserver = nullptr;
}

void ComputeThread::Start
(
	NNetModel       * const pModel,
	Param           * const pParam,
	SlowMotionRatio * const pSlowMotionRatio
) 
{
	m_pModel           = pModel;
	m_pParam           = pParam;
	m_pSlowMotionRatio = pSlowMotionRatio;
	m_pParam->AddParameterObserver( m_pTimeResObserver );       // notify me if parameters change
	m_pSlowMotionRatio->RegisterObserver( m_pTimeResObserver ); // notify me if slomo ratio changes
	m_pTimeResObserver->Notify( true );
	StartThread( L"ComputeThread", true ); 
}

void ComputeThread::RunComputation( )
{
	if ( ! m_bContinue )
	{
		m_bContinue = true;
		m_runObservable.NotifyAll( false);
		Reset( );
		m_runEvent.Continue();
	}
}

void ComputeThread::StopComputation( )
{
	if ( m_bContinue )
	{
		m_bContinue = false;
		m_runObservable.NotifyAll( false );
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
	m_hrTimer.Restart();
	m_pModel->ResetSimulationTime();
}

long ComputeThread::computeCyclesTodo( fMicroSecs const usTilStartRealTime )
{
	fMicroSecs const usNominalSimuTime { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
	fMicroSecs const usActualSimuTime  { m_pModel->GetSimulationTime( ) };                                // get actual time stamp
	fMicroSecs const usMissingSimuTime { usNominalSimuTime - usActualSimuTime };                          // compute missing simulation time
	long       const lCyclesTodo       { CastToLong(usMissingSimuTime / m_pParam->GetTimeResolution()) }; // compute # cycles to be computed
	return max( 0, lCyclesTodo );
}

void ComputeThread::compute() 
{
	fMicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
	long       const lCyclesTodo        { computeCyclesTodo( usTilStartRealTime ) }; // compute # cycles to be computed
	long             lCyclesDone        { 0 };
	while ( (lCyclesDone < lCyclesTodo) && m_bContinue )
	{
		if ( m_pModel->Compute( ) ) // returns true, if stop on trigger fires
			m_bContinue = false;
		++lCyclesDone;
	}

	fMicroSecs const usSpentInCompute { m_hrTimer.GetMicroSecsTilStart( ) - usTilStartRealTime };
	fMicroSecs const usSleepTime      { m_usRealTimeAvailPerCycle - usSpentInCompute };
	if ( usSleepTime > 10000.0_MicroSecs )
		Sleep( 10 );
	if ( lCyclesDone > 0 )
	{
		m_usRealTimeSpentPerCycle = usSpentInCompute / CastToFloat(lCyclesTodo);
		m_performanceObservable.NotifyAll( false);
	}
}

fMicroSecs ComputeThread::GetSimulationTime( ) const 
{ 
	return m_pModel->GetSimulationTime( ); 
}

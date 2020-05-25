// ComputeThread.cpp
//
// NNetSimu

#include "stdafx.h"
#include "NNetModel.h"
#include "NNetParameters.h"
#include "SlowMotionRatio.h"
#include "win32_fatalError.h"
#include "ComputeThread.h"

ComputeThread::ComputeThread
(
	NNetModel       * const pModel,
	Param           * const pParam,
	SlowMotionRatio * const pSlowMotionRatio

) :
	m_pNNetModel      ( pModel ),
	m_pParam          ( pParam ),
	m_pSlowMotionRatio( pSlowMotionRatio )
{
	m_pTimeResObserver = new TimeResObserver( this );
	m_pParam->AddParameterObserver( m_pTimeResObserver );   // notify me if parameters change
	m_pSlowMotionRatio->RegisterObserver( m_pTimeResObserver ); // notify ne if slomo ratio changes
	m_pTimeResObserver->Notify( true );
	StartThread( L"ComputeThread", true ); 
}

ComputeThread::~ComputeThread( )
{
	delete m_pTimeResObserver;
	m_pTimeResObserver = nullptr;
}

void ComputeThread::ThreadStartupFunc( )  // everything happens in startup function
{                                         // no thread messages used
	try
	{
		m_event.Wait( );
		for (;;)
		{
			if ( compute() )
				m_event.Wait( );
		}
	}
	catch ( ... )
	{
		FatalError::Happened( 1000, "ComputeThread" );
	}
}

void ComputeThread::RunComputation( )
{
	m_bContinue = true;
	m_runObservable.NotifyAll( false);
	m_hrTimer.Start();
	m_event.Continue();
}

void ComputeThread::StopComputation( )
{
	m_bContinue = false;
	m_runObservable.NotifyAll( false );
	m_hrTimer.Stop();
}

void ComputeThread::Reset( )
{
	m_hrTimer.Restart();
	m_pNNetModel->ResetSimulationTime();
}

bool ComputeThread::compute() 
{
	bool             bStop              { false };
	fMicroSecs const usTilStartRealTime { m_hrTimer.GetMicroSecsTilStart( ) };
	fMicroSecs const usTilStartSimuTime { m_pSlowMotionRatio->RealTime2SimuTime( usTilStartRealTime ) };
	fMicroSecs const usActualSimuTime   { m_pNNetModel->GetSimulationTime( ) };                             // get actual time stamp
	fMicroSecs const usMissingSimuTime  { usTilStartSimuTime - usActualSimuTime };                          // compute missing simulation time
	fMicroSecs const usSimuTimeTodo     { min( usMissingSimuTime, m_pParam->GetTimeResolution() ) };        // respect time slot (resolution)
	long       const lCyclesTodo        { CastToLong( usSimuTimeTodo / m_pParam->GetTimeResolution( ) ) };  // compute # cycles to be computed
	long             lCyclesDone        { 0 };
	while ( (lCyclesDone < lCyclesTodo) && ! bStop )
	{
		if ( m_pNNetModel->Compute( ) ) // returns true, if stop on trigger fires
			bStop = true;
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

	return bStop;
}

void ComputeThread::TimeResObserver::Notify( bool const bImmediate )
{
	m_pThread->m_usRealTimeAvailPerCycle = m_pThread->m_pSlowMotionRatio->SimuTime2RealTime( m_pThread->GetSimuTimeResolution() );
}

fMicroSecs ComputeThread::GetSimuTimeResolution( ) const 
{ 
	return m_pParam->GetTimeResolution( ); 
}

fMicroSecs ComputeThread::GetRealTimeTilStart( ) const 
{ 
	return m_hrTimer.GetMicroSecsTilStart( ); 
}

fMicroSecs ComputeThread::GetSimulationTime( ) const 
{ 
	return m_pNNetModel->GetSimulationTime( ); 
}

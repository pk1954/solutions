// ComputeThread.h
//
// NNetSimu

#pragma once

#include "Observable.h"
#include "ObserverInterface.h"
#include "win32_event.h"
#include "win32_hiResTimer.h"
#include "win32_thread.h"
#include "NNetParameters.h"

class NNetModel;
class SlowMotionRatio;

class ComputeThread: public Util::Thread, public ObserverInterface
{
public:

	void Start
	(
		NNetModel       * const,
		Param           * const,
		SlowMotionRatio * const,
		Observable      * const,
		Observable      * const
	);

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const ) { };
	virtual void Notify( bool const bImmediate ) { Reset( ); }

	void Reset( );
	void SingleStep( );
	void ReleaseComputationLock( );
	void LockComputation( );
	void RunComputation( );
	void StopComputation( );
	bool IsRunning() const { return ! m_bStopped; }

	fMicroSecs GetTimeAvailPerCycle ( ) const { return m_usRealTimeAvailPerCycle; }
	fMicroSecs GetTimeSpentPerCycle ( ) const { return m_usRealTimeSpentPerCycle; }
	fMicroSecs GetRealTimeTilStart  ( ) const { return m_hrTimer.GetMicroSecsTilStart(); }
	fMicroSecs GetSimuTimeResolution( ) const { return m_pParam->GetTimeResolution(); }
	fMicroSecs GetSimulationTime    ( ) const;

private:

	NNetModel        * m_pModel                  { nullptr };
	Param            * m_pParam                  { nullptr };
	SlowMotionRatio  * m_pSlowMotionRatio        { nullptr };
	Observable       * m_pRunObservable          { nullptr };
	Observable       * m_pPerformanceObservable  { nullptr };
	fMicroSecs         m_usRealTimeSpentPerCycle { 0.0_MicroSecs };
	fMicroSecs         m_usRealTimeAvailPerCycle { 0.0_MicroSecs };
	bool               m_bStopped                { true }; // visible to UI
	bool               m_bComputationLocked      { true }; // internal lock (short time)
	HiResTimer         m_hrTimer                 { };
	SRWLOCK            m_srwlStopped             { SRWLOCK_INIT };

	void runComputation( );
	void stopComputation( );
};

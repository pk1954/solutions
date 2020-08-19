// ComputeThread.h
//
// NNetWindows

#pragma once

#include "ObserverInterface.h"
#include "win32_hiResTimer.h"
#include "win32_thread.h"

class Param;
class NNetModel;
class Observable;
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
	virtual void ThreadMsgDispatcher( MSG const ) { }
	virtual void Notify( bool const );

	void SingleStep( );
	void ReleaseComputationLock( );
	void LockComputation( );
	void RunComputation( );
	void StopComputation( );
	bool IsRunning() const { return ! m_bStopped; }

	float      GetEffectiveSlowmo    ( ) const { return m_fEffectiveSlowMo; };
	fMicroSecs GetTimeSpentPerCycle  ( ) const { return m_usRealTimeSpentPerCycle; }
	fMicroSecs GetSimuTimeResolution ( ) const;
	fMicroSecs GetTimeAvailPerCycle  ( ) const;

private:

	NNetModel       * m_pModel                  { nullptr };
	Param           * m_pParam                  { nullptr };
	SlowMotionRatio * m_pSlowMotionRatio        { nullptr };
	Observable      * m_pRunObservable          { nullptr };
	Observable      * m_pPerformanceObservable  { nullptr };
	bool              m_bStopped                { true };          // visible to UI
	bool              m_bComputationLocked      { true };          // internal lock (short time)
	HiResTimer        m_hrTimer                 { };
	SRWLOCK           m_srwlStopped             { SRWLOCK_INIT };

	// gross - time including stops 
	// net   - time excluding stops == effective computation time

	// real times are counted starting with last reset

	fMicroSecs        m_usSimuTimeAtLastReset   { 0.0_MicroSecs };
	fMicroSecs        m_usRealTimeSpentPerCycle { 0.0_MicroSecs };

	Ticks             m_ticksNetRunning { 0 };
	Ticks             m_ticksAtLastRun  { 0 };

	float m_fEffectiveSlowMo { 0.0f };

	void runComputation( );
	void stopComputation( );
	void reset( );
};

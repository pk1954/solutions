// ComputeThread.ixx
//
// NNetWindows

module;

#include <memory>
#include <Windows.h>

export module NNetWin32:ComputeThread;

import Types;
import ObserverInterface;
import Observable;
import HiResTimer;
import Thread;
import SlowMotionRatio;
import NNetModel;
import ScanMatrix;

using std::unique_ptr;

export class ComputeThread: public ::Thread, public ObserverInterface
{
public:

	void Initialize
	(
		SlowMotionRatio * const,
		Observable      * const,
		Observable      * const,
		Observable      * const,
		Observable      * const
	);

	void Reset();
	void SetModelInterface(NNetModelWriterInterface * const);
	void ThreadStartupFunc() final;
	void ThreadMsgDispatcher(MSG const &) final { }
	void Notify(bool const) final;
	void SingleStep();
	void RunComputation();
	void StopComputation();
	void StartScan();
	void StartStimulus();
	bool IsRunning    () const { return !m_bStopped; }
	bool IsScanRunning() const { return m_pNMWI->IsScanRunning(); }
	bool ModelLocked  () const { return m_pNMWI->ModelLocked(); }

	fMicroSecs GetSimuTimeResolution() const { return m_usSimuTimeResolution; }
	fMicroSecs GetTimeSpentPerCycle () const { return m_usRealTimeSpentPerCycle; }
	fMicroSecs GetTimeAvailPerCycle () const { return m_usTimeAvailPerCycle; }
	int        GetScanNr            () const { return m_iScanNr; }

private:

	NNetModelWriterInterface * m_pNMWI          { nullptr };

	SlowMotionRatio * m_pSlowMotionRatio        { nullptr };
	Observable      * m_pRunObservable          { nullptr };
	Observable      * m_pPerformanceObservable  { nullptr };
	Observable      * m_pDynamicModelObservable { nullptr };
	Observable      * m_pLockModelObservable    { nullptr };

	bool              m_bStopped                { true };  // visible to UI
	HiResTimer        m_hrTimer                 { };
	SRWLOCK           m_srwlModel               { SRWLOCK_INIT };

	fMicroSecs        m_usSimuTimeAtLastReset   { 0.0_MicroSecs };
	fMicroSecs        m_usSimuTimeResolution    { 0.0_MicroSecs };
	fMicroSecs        m_usRealTimeSpentPerCycle { 0.0_MicroSecs };
	fMicroSecs        m_usTimeAvailPerCycle     { 0.0_MicroSecs };

	Ticks             m_ticksNetRunning    { 0 };
	Ticks             m_ticksAtLastRun     { 0 };
	Ticks             m_ticksBeforeCompute { 0 };

	unique_ptr<ScanMatrix> m_upScanMatrix { };
	int                    m_iScanNr      { 0 };

	void standardRun();
	void scanRun();
	void computeAndStopOnTrigger();
};

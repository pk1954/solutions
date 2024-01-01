// ComputeThread.ixx
//
// NNetWindows

module;

#include <memory>
#include <Windows.h>
#include <chrono>

export module NNetWin32:ComputeThread;

import Types;
import ObserverInterface;
import Observable;
import PerfCounter;
import Thread;
import SlowMotionRatio;
import NNetModel;
import ScanMatrix;

using namespace std::chrono;
using std::unique_ptr;

export class ComputeThread: public ::Thread, public ObserverInterface
{
public:

	~ComputeThread();

	void Initialize
	(
		SlowMotionRatio * const,
		Observable      * const,
		Observable      * const,
		Observable      * const,
		Observable      * const
	);

	void SetModelInterface(NNetModelWriterInterface * const);
	void ThreadMsgDispatcher(MSG const&) final;
	void DoGameStuff() final;
	bool IsInGameMode() const final { return IsRunning(); }
	void Notify(bool const) final;
	void SingleStep();
	void StartScan();
	void StartStimulus();
	bool IsRunning    () const { return !m_bStopped; }
	bool IsScanRunning() const { return m_pNMWI->IsScanRunning(); }
	bool ModelLocked  () const { return m_pNMWI->ModelLocked(); }

	fMicroSecs GetSimuTimeResolution() const { return m_usSimuTimeResolution; }
	fMicroSecs GetTimeSpentPerCycle () const { return m_usRealTimeSpentPerCycle; }
	fMicroSecs GetTimeAvailPerCycle () const { return m_usTimeAvailPerCycle; }
	int        GetScanNr            () const { return m_iScanNr; }

	static int const TM_START { 1 };
	static int const TM_STOP  { 2 };
	static int const TM_SCAN  { 3 };
	static int const TM_RESET { 4 };

private:

	NNetModelWriterInterface * m_pNMWI          { nullptr };

	SlowMotionRatio * m_pSlowMotionRatio        { nullptr };
	Observable      * m_pRunObservable          { nullptr };
	Observable      * m_pPerformanceObservable  { nullptr };
	Observable      * m_pDynamicModelObservable { nullptr };
	Observable      * m_pLockModelObservable    { nullptr };

	bool              m_bStopped                { true };  // visible to UI
	SRWLOCK           m_srwlModel               { SRWLOCK_INIT };

	fMicroSecs        m_usSimuTimeResolution    { 0.0_MicroSecs };
	fMicroSecs        m_usRealTimeSpentPerCycle { 0.0_MicroSecs };
	fMicroSecs        m_usTimeAvailPerCycle     { 0.0_MicroSecs };

	Ticks             m_ticksPerCycle;
	Ticks             m_ticksNextComputation;
	Ticks             m_ticksAtStart;

	unique_ptr<ScanMatrix> m_upScanMatrix      { };
	int                    m_iScanNr           { 0 };
	int                    m_iNrOfComputations { 0 };

	void scanRun();
	void computeAndStopOnTrigger();
	bool time4NextComputation() const;
};

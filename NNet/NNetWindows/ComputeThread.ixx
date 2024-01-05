// ComputeThread.ixx
//
// NNetWindows

module;

#include <memory>
#include <chrono>

export module NNetWin32:ComputeThread;

import Types;
import HiResTimer;
import HiResClockGen;
import ObserverInterface;
import Observable;
import PerfCounter;
import SlowMotionRatio;
import NNetModel;
import ScanMatrix;

using namespace std::chrono;
using std::unique_ptr;

export class ComputeThread: public ObserverInterface
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
	void DoGameStuff();
	bool IsInGameMode() const { return IsRunning(); }
	void Notify(bool const) final;
	void SingleStep();
	void StartScan();
	void StartComputation();
	void StopComputation();
	void Reset();
	void StartStimulus();
	bool IsRunning    () const { return m_bRunning; }
	bool IsScanRunning() const { return m_bScanRunning; }
	bool ModelLocked  () const { return m_pNMWI->ModelLocked(); }

	fMicroSecs GetSimuTimeResolution() const { return m_pNMWI ? m_pNMWI->TimeResolution() : 0._MicroSecs; }
	fMicroSecs GetTimeSpentPerCycle () const { return PerfCounter::TicksToMicroSecs(m_computeTimer.GetAverageActionTicks()); }
	fMicroSecs GetTimeAvailPerCycle () const { return GetSimuTimeResolution() * m_pSlowMotionRatio->GetNominalSlowMo(); }
	int        GetScanNr            () const { return m_iScanNr; }

private:

	NNetModelWriterInterface * m_pNMWI          { nullptr };

	SlowMotionRatio * m_pSlowMotionRatio        { nullptr };
	Observable      * m_pRunObservable          { nullptr };
	Observable      * m_pPerformanceObservable  { nullptr };
	Observable      * m_pDynamicModelObservable { nullptr };
	Observable      * m_pLockModelObservable    { nullptr };
	bool              m_bRunning                { false }; 
	bool              m_bScanRunning            { false };
	HiResClockGen     m_computeClockGen;
	HiResTimer        m_computeTimer;

	unique_ptr<ScanMatrix> m_upScanMatrix { };
	int                    m_iScanNr      { 0 };
	RasterPoint            m_rpScanRun;
	unique_ptr<ScanImage>  m_upScanImageSum;
	fMicroSecs             m_usSimuNextPixelScan;

	void scanNextPixel();
	void setRunning(bool const);
	void computeAndStopOnTrigger();
};

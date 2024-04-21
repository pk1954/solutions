// Compute.ixx
//
// NNetWindows

module;

#include <memory>
#include <chrono>
#include <vector>

export module NNetWin32:Compute;

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
using std::vector;

export class Compute: public ObserverInterface
{
public:

	~Compute();

	void Initialize
	(
		SlowMotionRatio * const,
		Observable      * const,
		Observable      * const,
		Observable      * const,
		ScanMatrix      * const
	);

	void SetModelInterface(NNetModelWriterInterface * const);
	void DoGameStuff();
	bool IsInGameMode() const { return IsRunning(); }
	void Notify(bool const) final;
	void SingleStep();
	void StartScan();
	void StopScan();
	void StartComputation();
	void StopComputation();
	void Reset();
	void StartStimulus();
	void ClearImages();
	bool IsRunning    () const { return m_bRunning; }
	bool IsScanRunning() const { return m_upSumImage.get(); }
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
	bool              m_bRunning                { false }; 
	HiResClockGen     m_computeClockGen;
	HiResTimer        m_computeTimer;

	unique_ptr<ScanImageRaw> m_upSingleImage;
	unique_ptr<ScanImageRaw> m_upSumImage;
	ScanMatrix             * m_pScanMatrix { nullptr };
	int                      m_iScanNr     { 0 };
	RasterPoint              m_rpScanRun;
	fMicroSecs               m_usSimuNextPixelScan;

	void startScanPass();
	void scanNextPixel();
	void finishScan();
	void setRunning(bool const);
};

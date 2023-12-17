// ComputeThread.cpp
//
// NNetWindows

module;

#include <memory>
#include <vector>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:ComputeThread;

import Win32_Util_Resource;
import BoolOp;
import SlowMotionRatio;
import Observable;
import WinManager;
import FatalError;
import HiResTimer;
import SimulationTime;
import NNetModel;
import NNetPreferences;
import SaveCast;

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::byte;

void ComputeThread::Initialize   // runs in main thread
(
	SlowMotionRatio * const pSlowMotionRatio,
	Observable      * const pRunObservable,
	Observable      * const pPerformanceObservable,
	Observable      * const pDynamicModelObservable,
	Observable      * const pLockModelObservable
)
{
	m_pRunObservable          = pRunObservable;
	m_pPerformanceObservable  = pPerformanceObservable;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pLockModelObservable    = pLockModelObservable;
	m_pSlowMotionRatio        = pSlowMotionRatio;
	AcquireSRWLockExclusive(&  m_srwlModel);  // main thread has access to model
	BeginThread(L"ComputeThread"); 
}

void ComputeThread::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
	Reset();
}

void ComputeThread::Notify(bool const bImmediate) // slowmo ratio or parameters have changed
{
	Reset();
	m_pRunObservable->NotifyAll(false);
}

void ComputeThread::Reset()
{
	m_usSimuTimeAtLastReset = SimulationTime::Get();
	m_ticksNetRunning       = Ticks(0);
	m_ticksAtLastRun        = m_hrTimer.ReadHiResTimer();
	m_usSimuTimeResolution  = m_pNMWI->GetParams().TimeResolution(); 
	m_usTimeAvailPerCycle   = m_usSimuTimeResolution * m_pSlowMotionRatio->GetNominalSlowMo(); 
}

void ComputeThread::StartStimulus()    // runs in main thread
{
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}

void ComputeThread::computeAndStopOnTrigger()
{
	if (!m_pSlowMotionRatio->MaxSpeed())
	{
		fMicroSecs const usSimuTimeSinceLastReset    { SimulationTime::Get() - m_usSimuTimeAtLastReset };
		Ticks      const ticksNet                    { m_ticksNetRunning + m_hrTimer.ReadHiResTimer() - m_ticksAtLastRun };
		fMicroSecs const usNetRealTimeSinceLastReset { m_hrTimer.TicksToMicroSecs(ticksNet) };
		fMicroSecs const usNominalSimuTime           { usNetRealTimeSinceLastReset / m_pSlowMotionRatio->GetNominalSlowMo() };
		fMicroSecs const usMissingSimuTime           { usNominalSimuTime - usSimuTimeSinceLastReset }; // compute missing simulation time
		if (usMissingSimuTime < m_usSimuTimeResolution)
			return;
	}
	if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
	{
		m_bStopped = true;
		m_pRunObservable->NotifyAll(false); // notify observers, that computation stopped
	}
	m_pDynamicModelObservable->NotifyAll(false);   // screen refresh, if possible
	Ticks const ticksPerCycle { m_hrTimer.ReadHiResTimer() - m_ticksBeforeCompute };
	m_usRealTimeSpentPerCycle = m_hrTimer.TicksToMicroSecs(ticksPerCycle);
    m_ticksBeforeCompute      = m_hrTimer.ReadHiResTimer();
	m_pSlowMotionRatio->SetMeasuredSlowMo(m_usRealTimeSpentPerCycle / m_usSimuTimeResolution);
	m_pPerformanceObservable->NotifyAll(false);
}

void ComputeThread::standardRun()
{
	while (!m_bStopped && !m_pNMWI->IsScanRunning())
		computeAndStopOnTrigger();
}

void ComputeThread::scanRun()
{
	bool            const bMaxSpeed      { m_pSlowMotionRatio->MaxSpeed(tBoolOp::opTrue) };
	RasterPoint     const imageSize      { m_upScanMatrix->Size() };
	fMicroSecs            usScanTime     { SimulationTime::Get() };
	int                   iNrOfRuns      { m_pNMWI->GetNrOfScans() };
	ScanImage           * pImageScreen   { m_pNMWI->GetScanImage() };
	unique_ptr<ScanImage> upScanImageSum { make_unique<ScanImage>(imageSize, 0.0_mV)};
	RasterPoint           rpRun;
	m_bStopped = false;
	for (m_iScanNr = 1; m_iScanNr <= iNrOfRuns; ++m_iScanNr)
	{
		WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
		pImageScreen->Set(0.0_mV);
		for (rpRun.m_y = 0; rpRun.m_y < imageSize.m_y; ++rpRun.m_y)  // loop over rows
		{
			for (rpRun.m_x = 0; rpRun.m_x < imageSize.m_x; ++rpRun.m_x)  // loop over columns
			{
				usScanTime += m_pNMWI->PixelScanTime();
				while (SimulationTime::Get() < usScanTime)
					computeAndStopOnTrigger();
				pImageScreen->Set(rpRun, m_upScanMatrix->Scan(rpRun));
				if (m_bStopped)  // forced termination of scan
					goto EXIT;
			}
			m_pDynamicModelObservable->NotifyAll(true);  // force screen refresh
		}
		*upScanImageSum.get() += *pImageScreen;
		WinManager::PostCommand2App(IDM_FINISHING_SCAN, m_iScanNr);
	}
	m_pNMWI->ReplaceScanImage(move(upScanImageSum));
	m_pDynamicModelObservable->NotifyAll(false);

	EXIT:
	m_pSlowMotionRatio->MaxSpeed(bMaxSpeed);
	m_pNMWI->SetScanRunning(false);
	Reset();
}

void ComputeThread::StartScan()      // runs in main thread
{
	m_upScanMatrix = make_unique<ScanMatrix>(m_pNMWI->GetScanRaster().Size());
	m_upScanMatrix->Fill(*m_pNMWI);
	m_pNMWI->CreateImage();
	m_pNMWI->SetScanRunning(true);
}

void ComputeThread::RunComputation()    // runs in main thread
{
	if (m_bStopped)
	{
		m_bStopped = false;
		m_ticksAtLastRun = m_hrTimer.ReadHiResTimer();
		m_pRunObservable->NotifyAll(false);
		ReleaseSRWLockExclusive(&m_srwlModel); // allow ComputeThread to run
	}
}

void ComputeThread::StopComputation()    // runs in main thread
{
	if (!m_bStopped)
	{
		m_bStopped = true;                      // computation will stop at the next opportunity
		AcquireSRWLockExclusive(&m_srwlModel);  // wait until ComputeThread has finished activities
		m_ticksNetRunning += m_hrTimer.ReadHiResTimer() - m_ticksAtLastRun;
		m_pRunObservable->NotifyAll(false);
	}
}

void ComputeThread::ThreadStartupFunc()  // everything happens in startup function
{                                        // no thread messages used
	for (;;)
	{
		AcquireSRWLockExclusive(&m_srwlModel);   // wait here until main thread allows access to model
	    m_ticksBeforeCompute = m_hrTimer.ReadHiResTimer();
		if (m_pNMWI && m_pNMWI->IsScanRunning())
		{
			scanRun();
		}
		else
		{
			standardRun();
		}
		ReleaseSRWLockExclusive(&m_srwlModel);  // release access to model 
	}
}

void ComputeThread::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll(false);
}

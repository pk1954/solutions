// ComputeThread.cpp
//
// NNetWindows

module;

#include <memory>
#include <cstddef>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:ComputeThread;

import SlowMotionRatio;
import Observable;
import WinManager;
import FatalError;
import HiResTimer;
import SimulationTime;
import NNetModel;
import SaveCast;

using std::unique_ptr;
using std::make_unique;
using std::byte;

void ComputeThread::Initialize
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
	AcquireSRWLockExclusive(& m_srwlStopped);
	BeginThread(L"ComputeThread"); 
}

void ComputeThread::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
//	SimulationTime::Set();
	Reset();
}

void ComputeThread::Notify(bool const bImmediate) // slowmo ratio or parameters have changed
{
	Reset();
	m_pRunObservable->NotifyAll(false);
}

void ComputeThread::Reset()
{
	LockComputation();
	m_usSimuTimeAtLastReset = SimulationTime::Get();
	m_ticksNetRunning       = Ticks(0);
	m_ticksAtLastRun        = m_hrTimer.ReadHiResTimer();
	m_usSimuTimeResolution  = m_pNMWI->GetParams().TimeResolution(); 
	m_usTimeAvailPerCycle   = m_pSlowMotionRatio->SimuTime2RealTime(m_usSimuTimeResolution); 
	UnlockComputation();
}

void ComputeThread::LockComputation()  // runs in main thread
{
	if (! m_bComputationLocked)
	{
		m_bComputationLocked = true;
		if (! m_bStopped)      // if not already haltet, halt now
			haltComputation();
	}
}

void ComputeThread::UnlockComputation()   // runs in main thread
{
	if (m_bComputationLocked)
	{
		m_bComputationLocked = false;
		if (! m_bStopped)  // both locks are released. We can start to run
			runComputation();
	}
}

void ComputeThread::RunStopComputation()    // runs in main thread
{
	m_bStopped = ! m_bStopped;
	if (! m_bComputationLocked)  // if not already haltet, halt now
	{
		if (m_bStopped)
			haltComputation();
		else
			runComputation();
	}
}

void ComputeThread::StopComputation()    // runs in main thread
{
	if (!m_bStopped)
	{
		m_bStopped = true;
		m_pRunObservable->NotifyAll(false);
		if (!m_bComputationLocked)  // if not already stopped, stop now
			haltComputation();
	}
}

void ComputeThread::StartStimulus()    // runs in main thread
{
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}

void ComputeThread::runComputation()    // runs in main thread
{
	m_ticksAtLastRun = m_hrTimer.ReadHiResTimer();
	m_pRunObservable->NotifyAll(false);
	ReleaseSRWLockExclusive(& m_srwlStopped); // allow ComputeThread to run
}

void ComputeThread::haltComputation()    // runs in main thread
{
	AcquireSRWLockExclusive(& m_srwlStopped); // wait until ComputeThread has finished activities
	m_ticksNetRunning += m_hrTimer.ReadHiResTimer() - m_ticksAtLastRun;
	m_pRunObservable->NotifyAll(false);
}

fMicroSecs ComputeThread::simuTimeSinceLastReset() const
{
	return SimulationTime::Get() - m_usSimuTimeAtLastReset;
};

fMicroSecs ComputeThread::netRealTimeSinceLastReset() const
{
	Ticks      ticksNet      { m_ticksNetRunning + m_hrTimer.ReadHiResTimer() - m_ticksAtLastRun };
	fMicroSecs usNetRealTime { m_hrTimer.TicksToMicroSecs(ticksNet) };
	return usNetRealTime;
}

long ComputeThread::getCyclesTodo() const
{
	fMicroSecs const usNominalSimuTime { m_pSlowMotionRatio->RealTime2SimuTime(netRealTimeSinceLastReset()) };
	fMicroSecs const usMissingSimuTime { usNominalSimuTime - simuTimeSinceLastReset() };          // compute missing simulation time
	long       const lCycles           { Cast2Long(usMissingSimuTime / m_usSimuTimeResolution) }; // compute # cycles to be computed
	return max(0, lCycles);
}

void ComputeThread::StandardRun()
{
	while (!(m_bStopped || m_bComputationLocked))
	{
		Ticks const ticksBeforeLoop { m_hrTimer.ReadHiResTimer() };

		long const lCyclesTodo { getCyclesTodo() };
		long       lCyclesDone { 0 };

		while ((lCyclesDone < lCyclesTodo) && !(m_bStopped || m_bComputationLocked))
		{
			if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
			{
				m_bStopped = true;
				m_pRunObservable->NotifyAll(false); // notify observers, that computation stopped
			}
			m_pDynamicModelObservable->NotifyAll(false);
			++lCyclesDone;
		}

		Ticks const ticksInLoop { m_hrTimer.ReadHiResTimer() - ticksBeforeLoop };

		if (lCyclesDone > 0)
		{
			m_usRealTimeSpentPerCycle = m_hrTimer.TicksToMicroSecs(ticksInLoop / lCyclesDone);
			m_fEffectiveSlowMo = netRealTimeSinceLastReset() / simuTimeSinceLastReset();
			m_pPerformanceObservable->NotifyAll(false);
		}

		fMicroSecs const usSpentInCompute { m_hrTimer.TicksToMicroSecs(ticksInLoop) };
		fMicroSecs const usSleepTime      { m_usTimeAvailPerCycle - usSpentInCompute };
		if (usSleepTime > 10000.0_MicroSecs)
			Sleep(10);
	}
}

void ComputeThread::StartScan()
{
	StopComputation();
	m_upScanMatrix = make_unique<ScanMatrix>(m_pNMWI->GetScanRaster().Size());
	m_pNMWI->Apply2AllC<Pipe>([this](Pipe const& p){ m_upScanMatrix->Add2list(p, m_pNMWI->GetScanRaster()); });
	m_pNMWI->CreateImage();
	m_pNMWI->SetScanRunning(true);
	m_pLockModelObservable->NotifyAll(false);
	RunStopComputation();
}

void ComputeThread::ScanRun()
{
	RasterPoint const imageSize  { m_upScanMatrix->Size() };
	fMicroSecs        usScanTime { SimulationTime::Get() };
	Vector2D<mV>    * pImage     { m_pNMWI->GetScanImage() };
	RasterPoint       rpRun;

	for (rpRun.m_y = 0; rpRun.m_y < imageSize.m_y; ++rpRun.m_y)  // loop over rows
	{
		for (rpRun.m_x = 0; rpRun.m_x < imageSize.m_x; ++rpRun.m_x)  // loop over columns
		{ 
			usScanTime += m_pNMWI->PixelScanTime();

			while (SimulationTime::Get() < usScanTime)
				m_pNMWI->Compute();

			pImage->Set(rpRun, m_upScanMatrix->Scan(rpRun));
			m_pDynamicModelObservable->NotifyAll(false);  // screen refresh, if possible

			if (m_bStopped)  // forced termination of scan
			{
				WinManager::PostCommand2App(IDM_REJECT_IMAGES);
				goto EXIT;
			}
		}
		m_pDynamicModelObservable->NotifyAll(true);  // force screen refresh
	} 

	*pImage *= 1.0f / pImage->GetMax().GetValue();
	m_pDynamicModelObservable->NotifyAll(false);

	EXIT:
	m_pNMWI->SetScanRunning(false);
	StopComputation();
}

void ComputeThread::ThreadStartupFunc()  // everything happens in startup function
{                                        // no thread messages used
	for (;;)
	{
		AcquireSRWLockExclusive(&m_srwlStopped);
		if (m_pNMWI && m_pNMWI->IsScanRunning())
		{
			ScanRun();
		}
		else
		{
			StandardRun();
		}
		ReleaseSRWLockExclusive(&m_srwlStopped);
	}
}

void ComputeThread::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll(false);
}

// ComputeThread.cpp
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <vector>
#include <Windows.h>
#include "Resource.h"
#include <chrono>

module NNetWin32:ComputeThread;

import Win32_Util_Resource;
import BoolOp;
import SlowMotionRatio;
import Observable;
import WinManager;
import FatalError;
import PerfCounter;
import SimulationTime;
import NNetModel;
import NNetPreferences;
import SaveCast;

using namespace std::chrono;
using std::chrono::milliseconds;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::byte;

ComputeThread::~ComputeThread()
{
	Terminate();
}

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
	StartThread(L"ComputeThread");
	SetThreadAffinityMask(0x2);
}

void ComputeThread::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
	PostThreadCmd(ComputeThread::TM_RESET);
}

void ComputeThread::Notify(bool const bImmediate) // slowmo ratio or parameters have changed
{
	PostThreadCmd(ComputeThread::TM_RESET);
	m_pRunObservable->NotifyAll(false);
}

void ComputeThread::StartStimulus()    // runs in main thread
{
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}

bool ComputeThread::time4NextComputation() const
{
	Ticks const now = PerfCounter::Read();
	if (m_pSlowMotionRatio->MaxSpeed())
		return true;
	else
	{
		if (now >= m_ticksNextComputation)
			return true;
	}
	return false;
}

void ComputeThread::computeAndStopOnTrigger()
{
	m_ticksNextComputation += m_ticksPerCycle;
	++m_iNrOfComputations;
	if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
	{
		m_bStopped = true;
		m_pRunObservable->NotifyAll(false); // notify observers, that computation stopped
	}
	m_pDynamicModelObservable->NotifyAll(false);   // screen refresh, if possible
	Ticks const ticksSinceStart { PerfCounter::Read() - m_ticksAtStart };
	m_usRealTimeSpentPerCycle = PerfCounter::TicksToMicroSecs(ticksSinceStart / m_iNrOfComputations);
	m_pSlowMotionRatio->SetMeasuredSlowMo(m_usRealTimeSpentPerCycle / m_usSimuTimeResolution);
	m_pPerformanceObservable->NotifyAll(false);
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
	PostThreadCmd(ComputeThread::TM_RESET);
}

void ComputeThread::StartScan()      // runs in main thread
{
	PostThreadCmd(TM_SCAN);
}

void ComputeThread::DoGameStuff()
{
	if (IsRunning() && time4NextComputation())
	{
		AcquireSRWLockExclusive(&m_srwlModel);   // wait here until main thread allows access to model
		computeAndStopOnTrigger();
		ReleaseSRWLockExclusive(&m_srwlModel);  // release access to model 
	}
}

void ComputeThread::ThreadMsgDispatcher(MSG const& msg)  
{             
	assert(msg.message == WM_COMMAND);
	switch (msg.wParam)
	{
	case TM_START:
		if (m_bStopped)
		{
			m_bStopped = false;
			m_pRunObservable->NotifyAll(false);
			ReleaseSRWLockExclusive(&m_srwlModel); // allow ComputeThread to run
			m_ticksAtStart         = PerfCounter::Read();
			m_ticksNextComputation = m_ticksAtStart;
			m_iNrOfComputations    = 0;
		}
		break;

	case TM_STOP:
		if (IsRunning())
		{
			m_bStopped = true;                      // computation will stop at the next opportunity
			AcquireSRWLockExclusive(&m_srwlModel);  // wait until ComputeThread has finished activities
			m_pRunObservable->NotifyAll(false);
		}
		break;

	case TM_SCAN:
		m_upScanMatrix = make_unique<ScanMatrix>(m_pNMWI->GetScanRaster().Size());
		m_upScanMatrix->Fill(*m_pNMWI);
		m_pNMWI->CreateImage();
		m_pNMWI->SetScanRunning(true);
		break;

	case TM_RESET:
		m_ticksAtStart         = PerfCounter::Read();
		m_iNrOfComputations    = 0;
		m_usSimuTimeResolution = m_pNMWI->GetParams().TimeResolution(); 
		m_usTimeAvailPerCycle  = m_usSimuTimeResolution * m_pSlowMotionRatio->GetNominalSlowMo();
		m_ticksPerCycle        = PerfCounter::MicroSecsToTicks(m_usTimeAvailPerCycle);
		break;

	default:
		assert(false);
	}
}

void ComputeThread::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll(false);
}

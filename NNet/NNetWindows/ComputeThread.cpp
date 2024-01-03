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

void ComputeThread::setRunning(bool const bMode)
{
	m_bRunning = bMode;
	m_pRunObservable->NotifyAll(false); // notify observers, that computation stopped
	m_computeTimer.BeforeAction();
}

void ComputeThread::computeAndStopOnTrigger()
{
	m_computeClockGen.Action();
	if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
		setRunning(false);
	m_computeTimer.AfterAction();
	m_computeTimer.BeforeAction();
	m_pDynamicModelObservable->NotifyAll(false);   // screen refresh, if possible
	m_pSlowMotionRatio->SetMeasuredSlowMo(GetTimeSpentPerCycle() / m_pNMWI->TimeResolution());
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
	setRunning(true);
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
				if (!IsRunning())  // forced termination of scan
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
	if (IsRunning() && m_computeClockGen.Time4NextAction())
	{
		AcquireSRWLockExclusive(&m_srwlModel);   // wait here until main thread allows access to model
		computeAndStopOnTrigger();
		ReleaseSRWLockExclusive(&m_srwlModel);  // release access to model 
	}
}

void ComputeThread::reset()
{
	if (m_pSlowMotionRatio->MaxSpeed())
		m_computeClockGen.MaxSpeed();
	else
		m_computeClockGen.Reset(PerfCounter::MicroSecsToTicks(GetTimeAvailPerCycle()));
	m_computeTimer.Reset();
}

void ComputeThread::ThreadMsgDispatcher(MSG const& msg)  
{             
	assert(msg.message == WM_COMMAND);
	switch (msg.wParam)
	{
	case TM_START:
		if (!IsRunning())
		{
			reset();
			setRunning(true);
			ReleaseSRWLockExclusive(&m_srwlModel); // allow ComputeThread to run
		}
		break;

	case TM_STOP:
		if (IsRunning())
		{
			setRunning(false);
			AcquireSRWLockExclusive(&m_srwlModel); 
		}
		break;

	case TM_SCAN:
		m_upScanMatrix = make_unique<ScanMatrix>(m_pNMWI->GetScanRaster().Size());
		m_upScanMatrix->Fill(*m_pNMWI);
		m_pNMWI->CreateImage();
		m_pNMWI->SetScanRunning(true);
		break;

	case TM_RESET:
		reset();
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

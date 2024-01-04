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

void ComputeThread::scanNextPixel()
{
	m_pNMWI->GetScanImage()->Set(m_rpScanRun, m_upScanMatrix->Scan(m_rpScanRun));
	if (++m_rpScanRun.m_x < m_upScanMatrix->Width())     // Next scan Pixel
		return;
	                                                     // Scan line finished
	m_rpScanRun.m_x = 0;
	m_pDynamicModelObservable->NotifyAll(true);          // force screen refresh
	if (++m_rpScanRun.m_y < m_upScanMatrix->Height())
		return;
	                                                     // Scan finished
	m_rpScanRun.m_y = 0;
	*m_upScanImageSum.get() += *m_pNMWI->GetScanImage();
	WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
	if (++m_iScanNr <= m_pNMWI->GetNrOfScans())
		return;
	                                                     // Scan series finished 
	WinManager::PostCommand2App(IDM_FINISHING_SCAN, m_iScanNr);
	m_pNMWI->ReplaceScanImage(move(m_upScanImageSum));
	m_pDynamicModelObservable->NotifyAll(false);
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
	if (m_pNMWI->IsScanRunning() && (SimulationTime::Get() >= m_usSimuNextPixelScan))
	{
		m_usSimuNextPixelScan = SimulationTime::Get() + m_pNMWI->PixelScanTime();
		scanNextPixel();
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
		m_rpScanRun = RasterPoint(0, 0);
		m_iScanNr = 1;
		m_upScanImageSum = make_unique<ScanImage>(m_upScanMatrix->Size(), 0.0_mV);
		WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
		m_pNMWI->GetScanImage()->Set(0.0_mV);
		m_usSimuNextPixelScan = SimulationTime::Get();
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

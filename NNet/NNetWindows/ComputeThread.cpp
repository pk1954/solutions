// ComputeThread.cpp
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <vector>
#include "Resource.h"
#include <chrono>

#include <iostream>

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

using std::wcout;
using std::endl;
using std::wstring;

ComputeThread::~ComputeThread() = default;

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
}

void ComputeThread::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
	Reset();
}

void ComputeThread::Notify(bool const bImmediate) // slowmo ratio or parameters have changed
{
	Reset();
	m_pRunObservable->NotifyAll();
}

void ComputeThread::StartStimulus()    // runs in main thread
{
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}

void ComputeThread::setRunning(bool const bMode)
{
	m_bRunning = bMode;
	m_pRunObservable->NotifyAll(); // notify observers, that computation stopped
	m_computeTimer.BeforeAction();
}

void ComputeThread::StartScan()      // runs in main thread
{
	m_upScanMatrix = make_unique<ScanMatrix>(m_pNMWI->GetScanRaster().Size());
	m_upScanMatrix->Fill(*m_pNMWI);
	m_pNMWI->CreateImage();
	m_rpScanRun = RasterPoint(0, 0);
	m_iScanNr = 1;
	m_upScanImageSum = make_unique<ScanImage>(m_upScanMatrix->Size(), 0.0_mV);
	WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
	Reset();
	m_pNMWI->GetScanImage()->Set(0.0_mV);
	m_usSimuNextPixelScan = SimulationTime::Get();
}

HiResTimer tX;

void ComputeThread::DoGameStuff()
{
	if (IsRunning() && m_computeClockGen.Time4NextAction())
	{
		tX.BeforeAction();
		m_computeClockGen.Action();
		if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
			setRunning(false);
		m_computeTimer.AfterAndBeforeAction();
		m_pDynamicModelObservable->NotifyAll();
		m_pSlowMotionRatio->SetMeasuredSlowMo(GetTimeSpentPerCycle() / m_pNMWI->TimeResolution());
		m_pPerformanceObservable->NotifyAll();
		tX.AfterAction();
	wcout << L"tX = " << tX.Average2wstring() << endl;
	wcout << L"xxxxxxxxxxxxxxxxxxxxxx" << endl;
	}

	if (IsScanRunning())
		scanNextPixel();
}

void ComputeThread::scanNextPixel()
{
	fMicroSecs usNow { SimulationTime::Get() };
	if (usNow < m_usSimuNextPixelScan)
		return;

	m_usSimuNextPixelScan = usNow + m_pNMWI->PixelScanTime();

	mV mVpixel { m_upScanMatrix->Scan(m_rpScanRun) };
	m_pNMWI->GetScanImage()->Set(m_rpScanRun, mVpixel);

	if (++m_rpScanRun.m_x < m_upScanMatrix->Width())     // Next scan Pixel
		return;
	                                                     // Scan line finished
	m_rpScanRun.m_x = 0;
	m_pDynamicModelObservable->NotifyAll(); 
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
	m_pDynamicModelObservable->NotifyAll();
	setRunning(false);
}

void ComputeThread::Reset()
{
	if (m_pSlowMotionRatio->MaxSpeed())
		m_computeClockGen.MaxSpeed();
	else
		m_computeClockGen.Reset(PerfCounter::MicroSecsToTicks(GetTimeAvailPerCycle()));
	m_computeTimer.Reset();
}

void ComputeThread::StartComputation()
{
	if (!IsRunning())
	{
		Reset();
		setRunning(true);
	}
}

void ComputeThread::StopComputation()
{
	if (IsScanRunning())
		m_upScanImageSum.release();;
	if (IsRunning())
		setRunning(false);
}

void ComputeThread::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll();
}

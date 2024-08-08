// Compute.cpp
//
// NNetWindows

module;

#include "Resource.h"

module NNetWin32:Compute;

import std;
import BoolOp;
import SlowMotionRatio;
import Observable;
import WinManager;
import FatalError;
import PerfCounter;
import SimulationTime;
import NNetModel;
import SaveCast;

using namespace std::chrono;
using std::chrono::milliseconds;
using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::byte;
using std::time_t;
using std::wcout;
using std::endl;
using std::wstring;

Compute::~Compute() = default;

void Compute::Initialize   // runs in main thread
(
	SlowMotionRatio * const pSlowMotionRatio,
	Observable      * const pRunObservable,
	Observable      * const pPerformanceObservable,
	Observable      * const pDynamicModelObservable
)
{
	m_pRunObservable          = pRunObservable;
	m_pPerformanceObservable  = pPerformanceObservable;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pSlowMotionRatio        = pSlowMotionRatio;
}

void Compute::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
	Reset();
}

void Compute::Notify(bool const bImmediate) // slowmo ratio or parameters have changed
{
	Reset();
	m_pRunObservable->NotifyAll();
}

void Compute::StartStimulus()
{
	m_pNMWI->GetSigGenSelected()->StartStimulus();
	m_pNMWI->AddEvent(EventType::stimulus);
}

void Compute::setRunning(bool const bMode)
{
	m_bRunning = bMode;
	m_pRunObservable->NotifyAll(); // notify observers, that computation stopped
	m_computeTimer.BeforeAction();
}

void Compute::startScanPass()
{
	m_pNMWI->ClearScanImage();
	m_rpScanRun = RasterPoint(0, 0);
	WinManager::PostCommand2App(IDM_STARTING_SCAN, ++m_iScanNr);
}

void Compute::StartScan()
{
	Reset();
	m_pNMWI->PrepareScanMatrix();
	m_pNMWI->CreateImage();
	m_pNMWI->AddEvent(EventType::startScan);
	m_upSingleImage = make_unique<RawImage>(m_pNMWI->GetScanAreaSize(), 0.0_mV);
	m_upSumImage    = make_unique<RawImage>(m_pNMWI->GetScanAreaSize(), 0.0_mV);
	m_iScanNr = 0;
	startScanPass();
	m_usSimuNextPixelScan = SimulationTime::Get();
}

void Compute::scanNextPixel()
{
	m_usSimuNextPixelScan = SimulationTime::Get() + m_pNMWI->PixelScanTime();

	m_upSingleImage->Set(m_rpScanRun, m_pNMWI->Scan(m_rpScanRun));

	if (++m_rpScanRun.m_x < m_pNMWI->GetScanAreaWidth())      // next scan Pixel
		return;
	                                                     // scan line finished
	m_rpScanRun.m_x = 0;
	m_pDynamicModelObservable->NotifyAll(); 
	if (++m_rpScanRun.m_y < m_pNMWI->GetScanAreaHeight())
		return;
	                                                     // scan pass finished
	*m_upSumImage.get() += *m_upSingleImage.get();
	startScanPass();
	if (m_iScanNr > m_pNMWI->GetNrOfScans())
		finishScan();                                    // scan series finished 

}

void Compute::finishScan()
{
	WinManager::PostCommand2App(IDM_FINISHING_SCAN);
	m_pNMWI->DensityCorrection(*m_upSumImage.get());
	m_upSumImage->Normalize(1.0f);
	m_pNMWI->ReplaceImage(move(m_upSumImage));
	m_pDynamicModelObservable->NotifyAll();
	StopScan();
	setRunning(false);
}

void Compute::DoGameStuff()
{
	if (IsRunning() && m_computeClockGen.Time4NextAction())
	{
		m_computeClockGen.Action();
		if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
			setRunning(false);
		m_computeTimer.AfterAndBeforeAction();
		m_pDynamicModelObservable->NotifyAll();
		m_pSlowMotionRatio->SetMeasuredSlowMo(GetTimeSpentPerCycle() / m_pNMWI->TimeResolution());
		m_pPerformanceObservable->NotifyAll();
	}

	if (IsScanRunning() && (SimulationTime::Get() >= m_usSimuNextPixelScan))
		scanNextPixel();
}

void Compute::Reset()
{
	if (m_pSlowMotionRatio->MaxSpeed())
		m_computeClockGen.MaxSpeed();
	else
		m_computeClockGen.Reset(PerfCounter::MicroSecsToTicks(GetTimeAvailPerCycle()));
	m_computeTimer.Reset();
}

void Compute::ClearImages()
{
	m_upSumImage.release();
	m_upSingleImage.release();
}

void Compute::StartComputation()
{
	if (!IsRunning())
	{
		Reset();
		setRunning(true);
	}
}

void Compute::StopComputation()
{
	if (IsRunning())
		setRunning(false);
}

void Compute::StopScan()
{
	ClearImages();
	m_rpScanRun= RasterPoint(0, 0);
	m_pNMWI->AddEvent(EventType::stopScan);
}

void Compute::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll();
}

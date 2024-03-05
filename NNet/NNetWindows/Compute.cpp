// Compute.cpp
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <vector>
#include "Resource.h"
#include <chrono>

#include <iostream>

module NNetWin32:Compute;

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

Compute::~Compute() = default;

void Compute::Initialize   // runs in main thread
(
	SlowMotionRatio * const pSlowMotionRatio,
	Observable      * const pRunObservable,
	Observable      * const pPerformanceObservable,
	Observable      * const pDynamicModelObservable,
	ScanMatrix      * const pScanMatrix
)
{
	m_pRunObservable          = pRunObservable;
	m_pPerformanceObservable  = pPerformanceObservable;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pSlowMotionRatio        = pSlowMotionRatio;
	m_pScanMatrix             = pScanMatrix;
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
}

void Compute::setRunning(bool const bMode)
{
	m_bRunning = bMode;
	m_pRunObservable->NotifyAll(); // notify observers, that computation stopped
	m_computeTimer.BeforeAction();
}

void Compute::StartScan()
{
	m_pScanMatrix->PrepareScanMatrix();
	m_pNMWI->CreateRawScanImage();
	m_upSingleImage = make_unique<ScanImageRaw>(m_pScanMatrix->Size(), 0.0_mV);
	m_upSumImage    = make_unique<ScanImageRaw>(m_pScanMatrix->Size(), 0.0_mV);
	m_rpScanRun = RasterPoint(0, 0);
	m_iScanNr = 1;
	WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
	Reset();
	m_pNMWI->ClearScanImage();
	m_usSimuNextPixelScan = SimulationTime::Get();
}

//HiResTimer tX;

void Compute::DoGameStuff()
{
	if (IsRunning() && m_computeClockGen.Time4NextAction())
	{
		//tX.BeforeAction();
		m_computeClockGen.Action();
		if (m_pNMWI->Compute()) // returns true, if StopOnTrigger fires
			setRunning(false);
		m_computeTimer.AfterAndBeforeAction();
		m_pDynamicModelObservable->NotifyAll();
		m_pSlowMotionRatio->SetMeasuredSlowMo(GetTimeSpentPerCycle() / m_pNMWI->TimeResolution());
		m_pPerformanceObservable->NotifyAll();
		//tX.AfterAction();
	    //wcout << L"tX = " << tX.Average2wstring() << endl;
	    //wcout << L"xxxxxxxxxxxxxxxxxxxxxx" << endl;
	}

	if (IsScanRunning() && (SimulationTime::Get() >= m_usSimuNextPixelScan))
		scanNextPixel();
}

void Compute::scanNextPixel()
{
	m_usSimuNextPixelScan = SimulationTime::Get() + m_pNMWI->PixelScanTime();

	mV mVpixel { m_pScanMatrix->Scan(m_rpScanRun) };
	m_upSingleImage->Set(m_rpScanRun, mVpixel);
	m_pNMWI->GetScanImage()->Set(m_rpScanRun, 255);

	if (++m_rpScanRun.m_x < m_pScanMatrix->Width())      // Next scan Pixel
		return;
	                                                     // Scan line finished
	m_rpScanRun.m_x = 0;
	m_pDynamicModelObservable->NotifyAll(); 
	if (++m_rpScanRun.m_y < m_pScanMatrix->Height())
		return;
	                                                     // Scan finished
	m_rpScanRun.m_y = 0;
	*m_upSumImage.get() += *m_upSingleImage.get();
	m_pNMWI->ClearScanImage();
	WinManager::PostCommand2App(IDM_STARTING_SCAN, m_iScanNr);
	if (++m_iScanNr <= m_pNMWI->GetNrOfScans())
		return;
	                                                     // Scan series finished 
	WinManager::PostCommand2App(IDM_FINISHING_SCAN, m_iScanNr);
	m_pScanMatrix->DensityCorrection(*m_upSumImage.get());
	m_upSumImage->Normalize();
	unique_ptr<ScanImageByte> upScanImageByte { make_unique<ScanImageByte>(m_upSumImage->GetSize())};
	m_upSumImage->VisitAllPixelsC
	(
		[this, &upScanImageByte](RasterPoint const& rp) 
		{ 
			ColIndex index { Cast2Byte(m_upSumImage->Get(rp).GetValue() * 255.0f) };
			upScanImageByte->Set(rp, index);  
		}
	);
	m_pNMWI->ReplaceScanImage(move(upScanImageByte));
	m_pDynamicModelObservable->NotifyAll();
	setRunning(false);
}

void Compute::Reset()
{
	if (m_pSlowMotionRatio->MaxSpeed())
		m_computeClockGen.MaxSpeed();
	else
		m_computeClockGen.Reset(PerfCounter::MicroSecsToTicks(GetTimeAvailPerCycle()));
	m_computeTimer.Reset();
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
	if (IsScanRunning())
		m_upSumImage.release();
	if (IsRunning())
		setRunning(false);
}

void Compute::SingleStep() 
{ 
	m_pNMWI->Compute();
	m_pDynamicModelObservable->NotifyAll();
}

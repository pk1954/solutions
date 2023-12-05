// SimulationControl.cpp
//
// NNetWindows

module;

#include <compare>
#include "Resource.h"
#include <Windows.h>

module NNetWin32:SimulationControl;

import NNetPreferences;
import Win32_Util_Resource;
import Win32_Util;
import StatusBar;
import SlowMotionControl;
import :ComputeThread;

void SimulationControl::Initialize
(
	StatusBar     * const pStatusBar,
	ComputeThread * const pComputeThread
) 
{
    m_pStatusBar     = pStatusBar;
	m_pComputeThread = pComputeThread;

//	m_hwndSingleStep = m_pStatusBar->AddButton(L"SingleStep", IDM_FORWARD,    BS_PUSHBUTTON);
	m_hwndRunStop    = m_pStatusBar->AddButton(L"   Run    ", IDM_RUN_STOP,   BS_PUSHBUTTON);
	m_hwndScan       = m_pStatusBar->AddButton(L"   Scan   ", IDM_SCAN, BS_PUSHBUTTON);

	Notify(true);
}

void SimulationControl::Notify(bool const bImmediate)
{
	bool const bIsRunning        { m_pComputeThread->IsRunning() };
	bool const bIsScanRunning    { m_pComputeThread->IsScanRunning() };
	bool const bScanImagePresent { m_pComputeThread->IsScanImagePresent() };
	bool const bScanAreaVisible  { NNetPreferences::ScanAreaVisible() };

	SetWindowText(m_hwndRunStop, bIsRunning ? L"Stop" : L"Run");
	EnableWindow(m_hwndSingleStep, !bIsRunning);
	SlowMotionControl::Enable(!bIsScanRunning);

	if (bScanAreaVisible)
	{
		if (bScanImagePresent)  // Locked
		{
			if (bIsRunning)  // Run mode (locked)
			{
				if (bIsScanRunning)  // Scan mode
					::SetDlgItemState(m_hwndScan, L" Scanning ", IDM_UNLOCK, false);
				else
					::SetDlgItemState(m_hwndScan, L"  Locked  ", IDM_UNLOCK, false);
			}
			else  // Locked mode - not running
				::SetDlgItemState(m_hwndScan, L"  Unlock  ", IDM_UNLOCK, true);
		}
		else  // Not locked
		{
			::SetDlgItemState(m_hwndScan, L"   Scan   ", IDM_SCAN, bIsRunning);
		}
		Show(m_hwndScan, true);
	}
	else
	{
		Show(m_hwndScan, false);
	}
}
